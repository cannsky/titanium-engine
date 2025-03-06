#pragma once

#ifndef ENGINE_DIR
#define ENGINE_DIR "../../../"
#endif

#include "EngineVulkanManager.hpp"
#include "EngineDebugManager.hpp"
#include "EngineBuffer.hpp"
#include "EngineTexture.hpp"
#include "EngineTransformComponent.hpp"

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <cassert>
#include <unordered_map>

#include <vulkan/vulkan.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

namespace TulparEngine {

    class EngineStaticModel {
    public:
        struct Vertex {
            glm::vec3 position{};
            glm::vec3 color{};
            glm::vec3 normal{};
            glm::vec2 uv{};
            uint32_t textureIndex{0};

            bool operator==(const Vertex& other) const {
                return position == other.position &&
                       color == other.color &&
                       normal == other.normal &&
                       uv == other.uv &&
                       textureIndex == other.textureIndex;
            }

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions() {
                std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
                bindingDescriptions[0].binding = 0;
                bindingDescriptions[0].stride = sizeof(Vertex);
                bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
                return bindingDescriptions;
            }

            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() {
                std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};

                attributeDescriptions.push_back({0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position)});
                attributeDescriptions.push_back({1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)});
                attributeDescriptions.push_back({2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal)});
                attributeDescriptions.push_back({3, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, uv)});

                return attributeDescriptions;
            }
        };

        EngineStaticModel(const std::string& filePath, const std::string& textureBaseDir) {
            loadModel(filePath, textureBaseDir);
            createVertexBuffer(vertices);
            createIndexBuffer(indices);
            createDescriptorSets();
        }

        ~EngineStaticModel() {
            // Cleanup handled by unique_ptr and RAII.
        }

        EngineStaticModel(const EngineStaticModel&) = delete;
        EngineStaticModel& operator=(const EngineStaticModel&) = delete;

        void bind(VkCommandBuffer commandBuffer) {
            VkBuffer buffers[] = { vertexBuffer->getBuffer() };
            VkDeviceSize offsets[] = { 0 };
            vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

            if (hasIndexBuffer) {
                vkCmdBindIndexBuffer(commandBuffer, indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
            }
        }

        void draw(VkCommandBuffer commandBuffer) {
            if (hasIndexBuffer) {
                vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
            } else {
                vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
            }
        }

        VkDescriptorSet getTextureDescriptorSet() const { return textureDescriptorSet; }

        // Update push constants if needed (e.g., for transforms)
        void UpdatePushConstantsWithScaling(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, const EngineTransformComponent& transform) {
            struct SimplePushConstantData {
                glm::mat4 modelMatrix{1.0f};
                glm::mat4 normalMatrix{1.0f};
            } push{};

            push.modelMatrix = transform.mat4();
            push.normalMatrix = glm::transpose(glm::inverse(push.modelMatrix));

            vkCmdPushConstants(
                commandBuffer,
                pipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                sizeof(SimplePushConstantData),
                &push
            );
        }

    private:
        std::vector<Vertex> vertices{};
        std::vector<uint32_t> indices{};
        std::vector<std::shared_ptr<EngineTexture>> textures;

        std::unique_ptr<EngineBuffer> vertexBuffer;
        std::unique_ptr<EngineBuffer> indexBuffer;

        uint32_t vertexCount = 0;
        uint32_t indexCount = 0;
        bool hasIndexBuffer = false;

        VkDescriptorSet textureDescriptorSet{VK_NULL_HANDLE};

        std::unique_ptr<Assimp::Importer> importer;
        const aiScene* scene = nullptr;

        void loadModel(const std::string& filePath, const std::string& textureBaseDir) {
            importer = std::make_unique<Assimp::Importer>();
            std::string fullPath = "/Users/cangok/Documents/workspace/Game Engine Super/" + filePath;
            EngineDebugManager::log("Loading static model from path: " + fullPath, DebugCategory::Model);

            const aiScene* scene = importer->ReadFile(fullPath,
                aiProcess_Triangulate |
                aiProcess_FlipUVs |
                aiProcess_CalcTangentSpace |
                aiProcess_GenSmoothNormals);

            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
                throw std::runtime_error("Assimp error: " + std::string(importer->GetErrorString()));
            }

            this->scene = scene;

            vertices.clear();
            indices.clear();

            processNode(scene->mRootNode, scene, textureBaseDir);
        }

        void processNode(aiNode* node, const aiScene* scene, const std::string& textureBaseDir) {
            // Process all the node's meshes
            for (unsigned int i = 0; i < node->mNumMeshes; i++) {
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                processMesh(mesh, scene, textureBaseDir);
            }
            // Recursively process child nodes
            for (unsigned int i = 0; i < node->mNumChildren; i++) {
                processNode(node->mChildren[i], scene, textureBaseDir);
            }
        }

        void processMesh(aiMesh* mesh, const aiScene* scene, const std::string& textureBaseDir) {
            std::vector<Vertex> meshVertices(mesh->mNumVertices);

            for (size_t i = 0; i < mesh->mNumVertices; i++) {
                Vertex &vertex = meshVertices[i];

                // Position
                vertex.position = {
                    mesh->mVertices[i].x,
                    mesh->mVertices[i].y,
                    mesh->mVertices[i].z
                };

                // Normal
                if (mesh->HasNormals()) {
                    vertex.normal = {
                        mesh->mNormals[i].x,
                        mesh->mNormals[i].y,
                        mesh->mNormals[i].z
                    };
                } else {
                    vertex.normal = {0.0f, 0.0f, 0.0f};
                }

                // UV
                if (mesh->mTextureCoords[0]) {
                    vertex.uv = {
                        mesh->mTextureCoords[0][i].x,
                        mesh->mTextureCoords[0][i].y
                    };
                } else {
                    vertex.uv = {0.0f, 0.0f};
                }

                // Color
                if (mesh->HasVertexColors(0)) {
                    vertex.color = {
                        mesh->mColors[0][i].r,
                        mesh->mColors[0][i].g,
                        mesh->mColors[0][i].b
                    };
                } else {
                    vertex.color = {1.0f, 1.0f, 1.0f};
                }

                // Texture index (assuming one texture per mesh for simplicity)
                vertex.textureIndex = (textures.empty()) ? 0 : static_cast<uint32_t>(textures.size() - 1);
            }

            // Add mesh vertices to global vertex list
            uint32_t vertexOffset = static_cast<uint32_t>(vertices.size());
            vertices.insert(vertices.end(), meshVertices.begin(), meshVertices.end());

            // Process indices
            for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
                aiFace face = mesh->mFaces[i];
                for (unsigned int j = 0; j < face.mNumIndices; j++) {
                    indices.push_back(vertexOffset + face.mIndices[j]);
                }
            }

            // Load textures for this mesh
            if (mesh->mMaterialIndex >= 0) {
                aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
                loadMaterialTextures(material, aiTextureType_DIFFUSE, textureBaseDir);
            }
        }

        void loadMaterialTextures(aiMaterial *material, aiTextureType type, const std::string &textureBaseDir) {
            for (unsigned int i = 0; i < material->GetTextureCount(type); i++) {
                aiString str;
                material->GetTexture(type, i, &str);
                std::string textureFilename(str.C_Str());
                std::string texturePath = "/Users/cangok/Documents/workspace/Game Engine Super/" + textureBaseDir;

                EngineDebugManager::log("Loading texture from path: " + texturePath, DebugCategory::Model);
                auto texture = EngineTexture::createTextureFromFile(texturePath);
                textures.push_back(texture);
            }

            // If no texture found, use a default texture
            if (material->GetTextureCount(type) == 0) {
                EngineDebugManager::log("No diffuse textures found. Using default texture.", DebugCategory::Model);
                std::string defaultTexturePath = "/Users/cangok/Documents/workspace/Game Engine Super/textures/default.png";
                auto defaultTexture = EngineTexture::createTextureFromFile(defaultTexturePath);
                textures.push_back(defaultTexture);
            }
        }

        void createVertexBuffer(const std::vector<Vertex> &vertices) {
            vertexCount = static_cast<uint32_t>(vertices.size());
            assert(vertexCount >= 3 && "Vertex count must be at least 3");
            VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
            uint32_t vertexSize = sizeof(vertices[0]);

            EngineBuffer stagingBuffer{
                vertexSize,
                vertexCount,
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
            };

            stagingBuffer.map();
            stagingBuffer.writeToBuffer((void *) vertices.data());

            vertexBuffer = std::make_unique<EngineBuffer>(
                vertexSize,
                vertexCount,
                VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
            );

            EngineVulkanManager::GetInstance().copyBuffer(stagingBuffer.getBuffer(), vertexBuffer->getBuffer(), bufferSize);
        }

        void createIndexBuffer(const std::vector<uint32_t> &indices) {
            indexCount = static_cast<uint32_t>(indices.size());
            hasIndexBuffer = indexCount > 0;

            if (!hasIndexBuffer) {
                return;
            }

            VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;
            uint32_t indexSize = sizeof(indices[0]);

            EngineBuffer stagingBuffer{
                indexSize,
                indexCount,
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
            };

            stagingBuffer.map();
            stagingBuffer.writeToBuffer((void *) indices.data());

            indexBuffer = std::make_unique<EngineBuffer>(
                indexSize,
                indexCount,
                VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
            );

            EngineVulkanManager::GetInstance().copyBuffer(stagingBuffer.getBuffer(), indexBuffer->getBuffer(), bufferSize);
        }

        void createDescriptorSets() {
            // Allocate and update descriptor set for texture (set = 1)
            VkDescriptorSetLayout textureSetLayout = EngineVulkanManager::GetInstance().getTextureDescriptorSetLayout();
            VkDescriptorPool descriptorPool = EngineVulkanManager::GetInstance().getTextureDescriptorPool();

            VkDescriptorSetAllocateInfo textureAllocInfo{};
            textureAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            textureAllocInfo.descriptorPool = descriptorPool;
            textureAllocInfo.descriptorSetCount = 1;
            textureAllocInfo.pSetLayouts = &textureSetLayout;

            if (vkAllocateDescriptorSets(EngineVulkanManager::GetInstance().getDevice(), &textureAllocInfo, &textureDescriptorSet) != VK_SUCCESS) {
                throw std::runtime_error("Failed to allocate texture descriptor set!");
            }

            if (!textures.empty()) {
                VkDescriptorImageInfo imageInfo = textures[0]->getDescriptorImageInfo();

                VkWriteDescriptorSet textureWrite{};
                textureWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                textureWrite.dstSet = textureDescriptorSet;
                textureWrite.dstBinding = 0; // Binding index for texture sampler
                textureWrite.dstArrayElement = 0;
                textureWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                textureWrite.descriptorCount = 1;
                textureWrite.pImageInfo = &imageInfo;

                vkUpdateDescriptorSets(
                    EngineVulkanManager::GetInstance().getDevice(),
                    1, &textureWrite,
                    0, nullptr);
            }
        }
    };
}
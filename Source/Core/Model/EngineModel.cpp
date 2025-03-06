#include "EngineModel.hpp"
#include "EngineUtils.hpp"

// Removed tinyobjloader includes as per your initial comment
// #define TINYOBJLOADER_IMPLEMENTATION
// #include <tiny_obj_loader.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <cassert>
#include <unordered_map>
#include <algorithm>
#include <iostream>

namespace std {
    template <>
    struct hash<TulparEngine::EngineModel::Vertex> {
        size_t operator()(TulparEngine::EngineModel::Vertex const &vertex) const {
            size_t seed = 0;
            TulparEngine::hashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
            return seed;
        }
    };
}

namespace TulparEngine {

    EngineModel::EngineModel(const std::string& filePath, const std::string& textureBaseDir) {
        loadModel(filePath, textureBaseDir);

        EngineDebugManager::log("Model loaded: " + filePath, DebugCategory::Model);
        EngineDebugManager::log("Vertex count: " + std::to_string(vertices.size()), DebugCategory::Model);

        createVertexBuffer(vertices);
        createIndexBuffer(indices);
        
        // Correctly assign rootNode after loading
        rootNode = importer->GetScene()->mRootNode;
        
        // Initialize bone matrices
        boneMatrices.resize(boneInfos.size(), glm::mat4(1.0f));
        
        // Create descriptor sets
        createDescriptorSets();

        // Initialize current animation index
        currentAnimationIndex = 2; // Set to -1 if no animations
    }

    EngineModel::~EngineModel() 
    {
        // 1. Wait for GPU to finish any in-flight commands that might use this model's resources.
        //    (This is a simple approach that blocks until the device is idle.)
        auto device = EngineVulkanManager::GetInstance().getDevice();
        vkDeviceWaitIdle(device);

        // 2. Free the descriptor sets referencing model resources (buffers, textures).
        //    Texture descriptor set
        if (textureDescriptorSet != VK_NULL_HANDLE) {
            VkDescriptorPool texturePool = EngineVulkanManager::GetInstance().getTextureDescriptorPool();
            vkFreeDescriptorSets(device, texturePool, 1, &textureDescriptorSet);
            textureDescriptorSet = VK_NULL_HANDLE;
        }

        //    Bone (skeletal) descriptor set
        if (bonesDescriptorSet != VK_NULL_HANDLE) {
            VkDescriptorPool bonesPool = EngineVulkanManager::GetInstance().getBonesDescriptorPool();
            vkFreeDescriptorSets(device, bonesPool, 1, &bonesDescriptorSet);
            bonesDescriptorSet = VK_NULL_HANDLE;
        }

        // 3. Destroy the model’s Vulkan buffers (bone, index, vertex).
        //    These unique_ptrs call ~EngineBuffer() which does vkDestroyBuffer + vkFreeMemory
        boneBuffer.reset();   // or if (boneBuffer) boneBuffer->destroy();
        indexBuffer.reset();
        vertexBuffer.reset();

        // 4. Destroy all textures.
        //    Each texture is a shared_ptr<EngineTexture>; its destructor calls vkDestroySampler, vkDestroyImageView, etc.
        for (auto& texture : textures) {
            texture.reset();
        }
        textures.clear();

        // 5. Cleanup Assimp importer and scene references if needed.
        //    (importer is typically a std::unique_ptr<Assimp::Importer>)
        importer.reset();

        // Done! 
        // Now all Vulkan resources for this model are properly torn down.
    }

    void EngineModel::createVertexBuffer(const std::vector<Vertex> &vertices) {
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

    void EngineModel::createIndexBuffer(const std::vector<uint32_t> &indices) {
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

    void EngineModel::bind(VkCommandBuffer commandBuffer) {
        VkBuffer buffers[] = { vertexBuffer->getBuffer() };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

        if (hasIndexBuffer) {
            vkCmdBindIndexBuffer(commandBuffer, indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
        }
    }

    void EngineModel::draw(VkCommandBuffer commandBuffer) {
        if (hasIndexBuffer) {
            vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
        } else {
            vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
        }
    }

    std::vector<VkVertexInputBindingDescription> EngineModel::Vertex::getBindingDescriptions() {
        std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
        bindingDescriptions[0].binding = 0;
        bindingDescriptions[0].stride = sizeof(Vertex);
        bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescriptions;
    }

    std::vector<VkVertexInputAttributeDescription> EngineModel::Vertex::getAttributeDescriptions() {
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};

        attributeDescriptions.push_back({0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position)});
        attributeDescriptions.push_back({1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)});
        attributeDescriptions.push_back({2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal)});
        attributeDescriptions.push_back({3, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, uv)});
        attributeDescriptions.push_back({4, 0, VK_FORMAT_R32G32B32A32_UINT, offsetof(Vertex, boneIDs)});
        attributeDescriptions.push_back({5, 0, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Vertex, boneWeights)});

        return attributeDescriptions;
    }

    void EngineModel::loadModel(const std::string &filePath, const std::string &textureBaseDir) {
        importer = std::make_unique<Assimp::Importer>();
        std::string fullPath = "/Users/cangok/Documents/workspace/Game Engine Super/" + filePath;
        EngineDebugManager::log("Loading model from path: " + fullPath, DebugCategory::Model);
        const aiScene* scene = importer->ReadFile(fullPath,
            aiProcess_Triangulate |
            aiProcess_FlipUVs |
            aiProcess_CalcTangentSpace |
            aiProcess_GenSmoothNormals);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            throw std::runtime_error("Assimp error: " + std::string(importer->GetErrorString()));
        }

        this->scene = scene; // Store the scene pointer for later use

        vertices.clear();
        indices.clear();

        processNode(scene->mRootNode, scene, textureBaseDir);

        coordinateFix = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //coordinateFix = glm::mat4(1.0f);

        glm::mat4 rootTransform = convertMatrix(scene->mRootNode->mTransformation);
        globalInverseTransform = glm::inverse(coordinateFix * rootTransform);
        //globalInverseTransform = glm::inverse(rootTransform);

        //  log bone names
        for (const auto& [name, index] : boneMapping) {
            EngineDebugManager::log("Bone: " + name + ", Index: " + std::to_string(index), DebugCategory::Model);
        }
    }

    void EngineModel::processNode(aiNode *node, const aiScene *scene, const std::string &textureBaseDir) {
        // Process all the node's meshes
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            processMesh(mesh, scene, textureBaseDir);
        }
        // Recursively process child nodes
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            processNode(node->mChildren[i], scene, textureBaseDir);
        }
    }

    void EngineModel::processMesh(aiMesh *mesh, const aiScene *scene, const std::string &textureBaseDir) {
        std::vector<Vertex> meshVertices(mesh->mNumVertices);

        // Initialize vertices
        for (size_t i = 0; i < mesh->mNumVertices; i++) {
            Vertex &vertex = meshVertices[i];

            // Positions
            vertex.position = {
                mesh->mVertices[i].x,
                mesh->mVertices[i].y,
                mesh->mVertices[i].z
            };

            // Normals
            if (mesh->HasNormals()) {
                vertex.normal = {
                    mesh->mNormals[i].x,
                    mesh->mNormals[i].y,
                    mesh->mNormals[i].z
                };
            } else {
                vertex.normal = {0.0f, 0.0f, 0.0f};
            }

            // Texture Coordinates
            if (mesh->mTextureCoords[0]) {
                vertex.uv = {
                    mesh->mTextureCoords[0][i].x,
                    mesh->mTextureCoords[0][i].y
                };
            } else {
                vertex.uv = {0.0f, 0.0f};
            }

            // Colors
            if (mesh->HasVertexColors(0)) {
                vertex.color = {
                    mesh->mColors[0][i].r,
                    mesh->mColors[0][i].g,
                    mesh->mColors[0][i].b
                };
            } else {
                vertex.color = {1.0f, 1.0f, 1.0f};
            }

            // Initialize bone IDs and weights
            std::fill(std::begin(vertex.boneIDs), std::end(vertex.boneIDs), 0);
            std::fill(std::begin(vertex.boneWeights), std::end(vertex.boneWeights), 0.0f);

            // Assign texture index (assuming one texture per mesh for simplicity)
            vertex.textureIndex = (textures.empty()) ? 0 : static_cast<uint32_t>(textures.size() - 1);
        }

        // Process bones
        for (unsigned int i = 0; i < mesh->mNumBones; i++) {
            aiBone* bone = mesh->mBones[i];
            std::string boneName(bone->mName.C_Str());

            uint32_t boneIndex = 0;
            if (boneMapping.find(boneName) == boneMapping.end()) {
                boneIndex = static_cast<uint32_t>(boneMapping.size());
                boneMapping[boneName] = boneIndex;

                BoneInfo boneInfo{};
                boneInfo.boneOffset = convertMatrix(bone->mOffsetMatrix);
                boneInfos.push_back(boneInfo);
            } else {
                boneIndex = boneMapping[boneName];
            }

            // Assign bone weights to vertices
            for (unsigned int j = 0; j < bone->mNumWeights; j++) {
                aiVertexWeight weight = bone->mWeights[j];
                uint32_t vertexId = weight.mVertexId;
                float boneWeight = weight.mWeight;

                if (vertexId >= meshVertices.size()) {
                    throw std::runtime_error("Vertex ID exceeds mesh vertex count");
                }

                Vertex &vertex = meshVertices[vertexId];
                addBoneData(vertex, boneIndex, boneWeight);
            }
        }

        // Normalize bone weights
        for (size_t i = 0; i < meshVertices.size(); ++i) {
            Vertex &vertex = meshVertices[i];
            float totalWeight = 0.0f;
            for (int j = 0; j < MAX_BONES_PER_VERTEX; j++) {
                totalWeight += vertex.boneWeights[j];
            }
            if (totalWeight > 0.0f) {
                for (int j = 0; j < MAX_BONES_PER_VERTEX; j++) {
                    vertex.boneWeights[j] /= totalWeight;
                }
            }
        }

        // Optimize bone weights by sorting and normalizing
        for (size_t i = 0; i < meshVertices.size(); ++i) {
            Vertex &vertex = meshVertices[i];
            // Create a vector of pairs (weight, boneID)
            std::vector<std::pair<float, uint32_t>> boneData;
            for (int j = 0; j < MAX_BONES_PER_VERTEX; j++) {
                boneData.emplace_back(vertex.boneWeights[j], vertex.boneIDs[j]);
            }

            // Sort in descending order based on weight
            std::sort(boneData.begin(), boneData.end(),
                [](const std::pair<float, uint32_t> &a, const std::pair<float, uint32_t> &b) {
                    return a.first > b.first;
                });

            // Assign sorted data back to vertex
            float totalWeight = 0.0f;
            for (int j = 0; j < MAX_BONES_PER_VERTEX; j++) {
                vertex.boneWeights[j] = boneData[j].first;
                vertex.boneIDs[j] = boneData[j].second;
                totalWeight += vertex.boneWeights[j];
            }

            if (totalWeight > 0.0f) {
                for (int j = 0; j < MAX_BONES_PER_VERTEX; ++j) {
                    vertex.boneWeights[j] /= totalWeight;
                }
            }
        }

        // Add mesh vertices to global vertices list
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

    void EngineModel::loadMaterialTextures(aiMaterial *material, aiTextureType type, const std::string &textureBaseDir) {
        for (unsigned int i = 0; i < material->GetTextureCount(type); i++) {
            aiString str;
            material->GetTexture(type, i, &str);
            std::string textureFilename(str.C_Str());
            std::string texturePath = "/Users/cangok/Documents/workspace/Game Engine Super/" + textureBaseDir;
            
            EngineDebugManager::log("Loading texture from path: " + texturePath, DebugCategory::Model);
            auto texture = EngineTexture::createTextureFromFile(texturePath);
            textures.push_back(texture);
        }

        // Handle case where no textures are found
        if (material->GetTextureCount(type) == 0) {
            // Optionally, assign a default texture
            if (!textures.empty()) {
                // Use the last texture index
                // Assuming textures[0] is a default texture
            } else {
                // Load a default texture or handle accordingly
                EngineDebugManager::log("No diffuse textures found. Using default texture.", DebugCategory::Model);
                std::string defaultTexturePath = "/Users/cangok/Documents/workspace/Game Engine Super/textures/default.png";
                auto defaultTexture = EngineTexture::createTextureFromFile(defaultTexturePath);
                textures.push_back(defaultTexture);
            }
        }
    }

    void EngineModel::addBoneData(Vertex &vertex, uint32_t boneID, float weight) {
        assert(boneID < boneInfos.size() && "Bone ID exceeds the number of bones");

        // Try to find an empty slot first
        for (int i = 0; i < MAX_BONES_PER_VERTEX; i++) {
            if (vertex.boneWeights[i] == 0.0f) {
                vertex.boneIDs[i] = boneID;
                vertex.boneWeights[i] = weight;
                return;
            }
        }

        // If no empty slot, find the slot with the smallest weight
        int smallestWeightIndex = 0;
        float smallestWeight = vertex.boneWeights[0];
        for (int i = 1; i < MAX_BONES_PER_VERTEX; i++) {
            if (vertex.boneWeights[i] < smallestWeight) {
                smallestWeight = vertex.boneWeights[i];
                smallestWeightIndex = i;
            }
        }

        // Replace if the new weight is greater than the smallest existing weight
        if (weight > smallestWeight) {
            vertex.boneIDs[smallestWeightIndex] = boneID;
            vertex.boneWeights[smallestWeightIndex] = weight;
        }
    }

    void EngineModel::collectAnimationBones(const aiScene* scene) {
        for (unsigned int i = 0; i < scene->mNumAnimations; ++i) {
            aiAnimation* aiAnim = scene->mAnimations[i];
            for (unsigned int j = 0; j < aiAnim->mNumChannels; ++j) {
                aiNodeAnim* aiNodeAnim = aiAnim->mChannels[j];
                std::string boneName(aiNodeAnim->mNodeName.C_Str());
                
                // **Issue**: Adding bones from animations that aren't in mesh bones
                // **Solution**: Remove this block to prevent inconsistency
                /*
                if (boneMapping.find(boneName) == boneMapping.end()) {
                    uint32_t boneIndex = static_cast<uint32_t>(boneMapping.size());
                    boneMapping[boneName] = boneIndex;

                    BoneInfo boneInfo{};
                    boneInfo.boneOffset = glm::mat4(1.0f); // Incorrect
                    boneInfos.push_back(boneInfo);
                }
                */
                
                // **Alternative Solution**: Log a warning if an animation references a non-existent bone
                if (boneMapping.find(boneName) == boneMapping.end()) {
                    EngineDebugManager::log("Warning: Animation references a bone (" + boneName + ") not present in mesh bones.", DebugCategory::Model);
                    // Optionally, handle this case appropriately, e.g., skip the animation channel
                }
            }
        }
    }

    void EngineModel::processAnimations(const aiScene* scene) {
        for (unsigned int i = 0; i < scene->mNumAnimations; ++i) {
            aiAnimation* aiAnim = scene->mAnimations[i];
            EngineAnimation animation;
            animation.name = aiAnim->mName.C_Str();
            if (animation.name.empty()) {
                animation.name = "Animation_" + std::to_string(animations.size());
            }
            animation.duration = static_cast<float>(aiAnim->mDuration);
            animation.ticksPerSecond = aiAnim->mTicksPerSecond != 0.0f ? static_cast<float>(aiAnim->mTicksPerSecond) : 25.0f; // Default to 25 if not specified

            EngineDebugManager::log("Processing animation: " + animation.name, DebugCategory::Model);

            for (unsigned int j = 0; j < aiAnim->mNumChannels; ++j) {
                aiNodeAnim* aiNodeAnim = aiAnim->mChannels[j];
                BoneAnimation boneAnim;
                std::string boneName = aiNodeAnim->mNodeName.C_Str();
                boneAnim.boneName = boneName;

                // Check if bone exists in boneMapping
                if (boneMapping.find(boneName) == boneMapping.end()) {
                    EngineDebugManager::log("Warning: Animation references a bone (" + boneName + ") not present in mesh bones. Skipping this bone's animation channel.", DebugCategory::Model);
                    continue; // Skip this bone's animation channel
                }

                // Process position keys
                for (unsigned int k = 0; k < aiNodeAnim->mNumPositionKeys; ++k) {
                    KeyPosition kp;
                    kp.time = static_cast<float>(aiNodeAnim->mPositionKeys[k].mTime);
                    kp.position = glm::vec3(
                        aiNodeAnim->mPositionKeys[k].mValue.x,
                        aiNodeAnim->mPositionKeys[k].mValue.y,
                        aiNodeAnim->mPositionKeys[k].mValue.z
                    );
                    boneAnim.positions.push_back(kp);
                }

                // Process rotation keys
                for (unsigned int k = 0; k < aiNodeAnim->mNumRotationKeys; ++k) {
                    KeyRotation kr;
                    kr.time = static_cast<float>(aiNodeAnim->mRotationKeys[k].mTime);
                    aiQuaternion aiQuat = aiNodeAnim->mRotationKeys[k].mValue;
                    kr.rotation = glm::quat(aiQuat.w, aiQuat.x, aiQuat.y, aiQuat.z);
                    boneAnim.rotations.push_back(kr);
                }

                // Process scaling keys
                for (unsigned int k = 0; k < aiNodeAnim->mNumScalingKeys; ++k) {
                    KeyScaling ks;
                    ks.time = static_cast<float>(aiNodeAnim->mScalingKeys[k].mTime);
                    ks.scale = glm::vec3(
                        aiNodeAnim->mScalingKeys[k].mValue.x,
                        aiNodeAnim->mScalingKeys[k].mValue.y,
                        aiNodeAnim->mScalingKeys[k].mValue.z
                    );
                    boneAnim.scalings.push_back(ks);
                }
                animation.boneAnimations.push_back(boneAnim);
            }

            // log animation bones and indexes
            for (const auto& [name, index] : boneMapping) {
                EngineDebugManager::log("Anim Bone: " + name + ", Index: " + std::to_string(index), DebugCategory::Model);
            }

            animations.push_back(animation);
        }

        EngineDebugManager::log("Total animations loaded: " + std::to_string(animations.size()), DebugCategory::Model);
    }

    void EngineModel::loadAnimationsFromFile(const std::string& animationFilePath) {
        Assimp::Importer animationImporter;
        std::string fullPath = "/Users/cangok/Documents/workspace/Game Engine Super/" + animationFilePath;
        EngineDebugManager::log("Loading animation from file: " + fullPath, DebugCategory::Model);
        
        // Load the animation file. Use appropriate post-processing flags.
        // Since it's an animation file, some flags like triangulation might be unnecessary,
        // but including them for consistency.
        const aiScene* animScene = animationImporter.ReadFile(fullPath,
            aiProcess_Triangulate | 
            aiProcess_FlipUVs | 
            aiProcess_CalcTangentSpace | 
            aiProcess_GenSmoothNormals);
        
        if (!animScene || animScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !animScene->mRootNode) {
            throw std::runtime_error("Assimp error while loading animation: " + std::string(animationImporter.GetErrorString()));
        }
        
        if (animScene->mNumAnimations == 0) {
            EngineDebugManager::log("No animations found in file: " + animationFilePath, DebugCategory::Model);
            return;
        }
        
        // Process animations from the loaded file
        processAnimations(animScene);
        
        EngineDebugManager::log("Loaded " + std::to_string(animScene->mNumAnimations) + " animation(s) from: " + animationFilePath, DebugCategory::Model);
    }

    size_t EngineModel::findPositionKey(const std::vector<KeyPosition>& positions, float time) const {
        if (positions.empty()) {
            throw std::runtime_error("No position keys available for interpolation.");
        }

        for (size_t i = 0; i < positions.size() - 1; ++i) {
            if (time < positions[i + 1].time) {
                return i;
            }
        }
        return positions.size() - 1;
    }

    size_t EngineModel::findRotationKey(const std::vector<KeyRotation>& rotations, float time) const {
        if (rotations.empty()) {
            throw std::runtime_error("No rotation keys available for interpolation.");
        }

        for (size_t i = 0; i < rotations.size() - 1; ++i) {
            if (time < rotations[i + 1].time) {
                return i;
            }
        }
        return rotations.size() - 1;
    }

    size_t EngineModel::findScalingKey(const std::vector<KeyScaling>& scalings, float time) const {
        if (scalings.empty()) {
            throw std::runtime_error("No scaling keys available for interpolation.");
        }

        for (size_t i = 0; i < scalings.size() - 1; ++i) {
            if (time < scalings[i + 1].time) {
                return i;
            }
        }
        return scalings.size() - 1;
    }

    // Interpolation Helper Functions

    glm::mat4 EngineModel::interpolatePosition(const std::vector<KeyPosition>& positions, float time) const {
        if (positions.empty()) return glm::mat4(1.0f);
        if (positions.size() == 1) return glm::translate(glm::mat4(1.0f), positions[0].position);

        size_t kp0_index = findPositionKey(positions, time);
        size_t kp1_index = (kp0_index + 1 < positions.size()) ? kp0_index + 1 : kp0_index;

        const KeyPosition& kp0 = positions[kp0_index];
        const KeyPosition& kp1 = positions[kp1_index];

        float deltaTime = kp1.time - kp0.time;
        float factor = (deltaTime > 0.0f) ? (time - kp0.time) / deltaTime : 0.0f;
        factor = glm::clamp(factor, 0.0f, 1.0f);

        glm::vec3 interpolatedPos = glm::mix(kp0.position, kp1.position, factor);
        return glm::translate(glm::mat4(1.0f), interpolatedPos);
    }

    glm::mat4 EngineModel::interpolateRotation(const std::vector<KeyRotation>& rotations, float time) const {
        if (rotations.empty()) return glm::mat4(1.0f);
        if (rotations.size() == 1) return glm::mat4_cast(rotations[0].rotation);

        size_t kr0_index = findRotationKey(rotations, time);
        size_t kr1_index = (kr0_index + 1 < rotations.size()) ? kr0_index + 1 : kr0_index;

        const KeyRotation& kr0 = rotations[kr0_index];
        const KeyRotation& kr1 = rotations[kr1_index];

        float deltaTime = kr1.time - kr0.time;
        float factor = (deltaTime > 0.0f) ? (time - kr0.time) / deltaTime : 0.0f;
        factor = glm::clamp(factor, 0.0f, 1.0f);

        glm::quat interpolatedRot = glm::slerp(kr0.rotation, kr1.rotation, factor);
        interpolatedRot = glm::normalize(interpolatedRot);
        return glm::mat4_cast(interpolatedRot);
    }

    glm::mat4 EngineModel::interpolateScaling(const std::vector<KeyScaling>& scalings, float time) const {
        if (scalings.empty()) return glm::mat4(1.0f);
        if (scalings.size() == 1) return glm::scale(glm::mat4(1.0f), scalings[0].scale);

        size_t ks0_index = findScalingKey(scalings, time);
        size_t ks1_index = (ks0_index + 1 < scalings.size()) ? ks0_index + 1 : ks0_index;

        const KeyScaling& ks0 = scalings[ks0_index];
        const KeyScaling& ks1 = scalings[ks1_index];

        float deltaTime = ks1.time - ks0.time;
        float factor = (deltaTime > 0.0f) ? (time - ks0.time) / deltaTime : 0.0f;
        factor = glm::clamp(factor, 0.0f, 1.0f);

        glm::vec3 interpolatedScale = glm::mix(ks0.scale, ks1.scale, factor);
        return glm::scale(glm::mat4(1.0f), interpolatedScale);
    }

    const aiNode* EngineModel::findFinalBoneNode(const aiNode* node) {
        std::string nodeName = node->mName.C_Str();
        // If this node does not contain $AssimpFbx$, assume it's the final bone node
        if (nodeName.find("$AssimpFbx$") == std::string::npos) {
            return node;
        }

        // Otherwise, it’s an intermediate node. Check its children.
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            const aiNode* finalNode = findFinalBoneNode(node->mChildren[i]);
            if (finalNode) {
                return finalNode; // return as soon as we find a suitable final node
            }
        }

        // If no suitable node is found, return nullptr.
        return nullptr;
    }

    void EngineModel::traverseNodeHierarchy(aiNode* node, const glm::mat4& parentTransform, const EngineAnimation& currentAnim) {
        glm::mat4 nodeTransform = convertMatrix(node->mTransformation);

        // Find matching bone animation channel if available
        auto it = std::find_if(
            currentAnim.boneAnimations.begin(),
            currentAnim.boneAnimations.end(),
            [&](const BoneAnimation& ba) { return ba.boneName == node->mName.C_Str(); }
        );

        if (it != currentAnim.boneAnimations.end()) {
            const BoneAnimation& boneAnim = *it;

            glm::mat4 translation = interpolatePosition(boneAnim.positions, currentTime);
            glm::mat4 rotation = interpolateRotation(boneAnim.rotations, currentTime);
            glm::mat4 scaling = interpolateScaling(boneAnim.scalings, currentTime);

            // Combine transformations
            nodeTransform = translation * rotation * scaling;
        }

        glm::mat4 globalTransform = parentTransform * nodeTransform;

        // Resolve the final bone node:
        const aiNode* finalBoneNode = findFinalBoneNode(node);
        if (finalBoneNode) {
            std::string finalBoneName = finalBoneNode->mName.C_Str();

            // Check if this final node matches one of our known bones
            auto boneIt = boneMapping.find(finalBoneName);
            if (boneIt != boneMapping.end()) {
                uint32_t boneIndex = boneIt->second;
                boneMatrices[boneIndex] = globalInverseTransform * globalTransform * boneInfos[boneIndex].boneOffset;
            }
        }

        // Recurse through children
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            traverseNodeHierarchy(node->mChildren[i], globalTransform, currentAnim);
        }
    }

    void EngineModel::updateAnimation(float deltaTime) {
        if (animations.empty() || currentAnimationIndex == -1) return;

        EngineAnimation& currentAnim = animations[currentAnimationIndex];
        currentTime += deltaTime * currentAnim.ticksPerSecond;
        if (currentTime > currentAnim.duration) {
            currentTime = fmod(currentTime, currentAnim.duration); // Loop the animation
        }

        // Reset bone matrices to identity before applying transformations
        std::fill(boneMatrices.begin(), boneMatrices.end(), glm::mat4(1.0f));

        // Traverse the node hierarchy and apply transformations
        traverseNodeHierarchy(rootNode, glm::mat4(1.0f), currentAnim);

        // Update the uniform buffer with new bone matrices
        if (boneBuffer) {
            boneBuffer->writeToBuffer(boneMatrices.data());
        }
    }

    void EngineModel::createDescriptorSets() {
        // Allocate and update descriptor set for texture (set = 1)
        VkDescriptorSetLayout textureSetLayout = EngineVulkanManager::GetInstance().getTextureDescriptorSetLayout();
        VkDescriptorPool descriptorPool = EngineVulkanManager::GetInstance().getTextureDescriptorPool();

        // Allocate texture descriptor set
        VkDescriptorSetAllocateInfo textureAllocInfo{};
        textureAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        textureAllocInfo.descriptorPool = descriptorPool;
        textureAllocInfo.descriptorSetCount = 1;
        textureAllocInfo.pSetLayouts = &textureSetLayout;

        if (vkAllocateDescriptorSets(EngineVulkanManager::GetInstance().getDevice(), &textureAllocInfo, &textureDescriptorSet) != VK_SUCCESS) {
            throw std::runtime_error("Failed to allocate texture descriptor set!");
        }

        // Update texture descriptor set
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

        // Allocate and update descriptor set for bones (set = 2)
        VkDescriptorSetLayout bonesSetLayout = EngineVulkanManager::GetInstance().getBonesDescriptorSetLayout();
        VkDescriptorPool bonesDescriptorPool = EngineVulkanManager::GetInstance().getBonesDescriptorPool();

        VkDescriptorSetAllocateInfo bonesAllocInfo{};
        bonesAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        bonesAllocInfo.descriptorPool = bonesDescriptorPool;
        bonesAllocInfo.descriptorSetCount = 1;
        bonesAllocInfo.pSetLayouts = &bonesSetLayout;

        if (vkAllocateDescriptorSets(EngineVulkanManager::GetInstance().getDevice(), &bonesAllocInfo, &bonesDescriptorSet) != VK_SUCCESS) {
            throw std::runtime_error("Failed to allocate bones descriptor set!");
        }

        // Update bone matrices uniform buffer
        VkDeviceSize bufferSize = sizeof(glm::mat4) * boneMatrices.size();
        // Ensure bufferSize is a multiple of minUniformBufferOffsetAlignment
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(EngineVulkanManager::GetInstance().getPhysicalDevice(), &deviceProperties);
        VkDeviceSize alignedSize = (bufferSize + deviceProperties.limits.minUniformBufferOffsetAlignment - 1) & 
                                   ~(deviceProperties.limits.minUniformBufferOffsetAlignment - 1);

        boneBuffer = std::make_unique<EngineBuffer>(
            sizeof(glm::mat4),
            static_cast<uint32_t>(boneMatrices.size()),
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        );

        boneBuffer->map();
        boneBuffer->writeToBuffer(boneMatrices.data());

        // Update bones descriptor set
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = boneBuffer->getBuffer();
        bufferInfo.offset = 0;
        bufferInfo.range = bufferSize;

        VkWriteDescriptorSet boneWrite{};
        boneWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        boneWrite.dstSet = bonesDescriptorSet;
        boneWrite.dstBinding = 0; // Binding index for bone matrices
        boneWrite.dstArrayElement = 0;
        boneWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        boneWrite.descriptorCount = 1;
        boneWrite.pBufferInfo = &bufferInfo;

        vkUpdateDescriptorSets(
            EngineVulkanManager::GetInstance().getDevice(),
            1, &boneWrite,
            0, nullptr);
    }

    void EngineModel::setBoneMatrices(const std::vector<glm::mat4>& boneTransforms) {
        assert(boneTransforms.size() <= boneMatrices.size() && "Bone transforms exceed the size of bone matrices");

        boneMatrices = boneTransforms;
        // Update the uniform buffer
        if (boneBuffer) {
            boneBuffer->writeToBuffer(boneMatrices.data());
        }
    }

    void EngineModel::UpdatePushConstantsWithScaling(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, const EngineTransformComponent& transform) {
        SimplePushConstantData push{};

        // Create a scaling matrix to flip the model vertically along the Y-axis
        glm::mat4 scaleY = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, -1.0f, 1.0f));
        // Create a scaling matrix to flip the model horizontally along the Z-axis
        glm::mat4 scaleZ = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, -1.0f));

        // Combine the original model matrix with the scaling matrix
        push.modelMatrix = transform.mat4();
        // Compute the normal matrix (transpose of the inverse of the model matrix)
        push.normalMatrix = glm::transpose(glm::inverse(push.modelMatrix));

        // Update push constants
        vkCmdPushConstants(
            commandBuffer,
            pipelineLayout,
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
            0,
            sizeof(SimplePushConstantData),
            &push
        );
    }
}
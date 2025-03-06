#pragma once

#ifndef ENGINE_DIR
#define ENGINE_DIR "../../../"
#endif

#include "EngineVulkanManager.hpp"
#include "EngineDebugManager.hpp"

#include "EngineBuffer.hpp"
#include "EngineTexture.hpp"
#include "EngineAnimation.hpp"

#include "EngineModelStruct.hpp"

#include "EngineTransformComponent.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/matrix4x4.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include <vulkan/vulkan.h>

namespace TulparEngine {
    class EngineModel {
    public:
        static const int MAX_BONES_PER_VERTEX = 4; // Maximum number of bone influences per vertex

        struct Vertex {
            glm::vec3 position{};
            glm::vec3 color{};
            glm::vec3 normal{};
            glm::vec2 uv{};
            uint32_t textureIndex{};
            uint32_t boneIDs[MAX_BONES_PER_VERTEX]{};
            float boneWeights[MAX_BONES_PER_VERTEX]{};

            // Returns binding descriptions for the vertex input
            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();

            // Returns attribute descriptions for the vertex input
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

            bool operator==(const Vertex& other) const {
                return position == other.position &&
                       color == other.color &&
                       normal == other.normal &&
                       uv == other.uv &&
                       std::equal(std::begin(boneIDs), std::end(boneIDs), std::begin(other.boneIDs)) &&
                       std::equal(std::begin(boneWeights), std::end(boneWeights), std::begin(other.boneWeights));
            }
        };

        struct BoneInfo {
            glm::mat4 boneOffset;          // Offset matrix transforms from model space to bone space
            glm::mat4 finalTransformation; // Final transformation matrix after animation
        };
        
        // Constructor: Loads a model from filePath with textures located in textureBaseDir
        EngineModel(const std::string& filePath, const std::string& textureBaseDir);

        EngineModel();
        
        // Destructor: Cleans up Vulkan resources
        ~EngineModel();

        // Delete copy constructor and copy assignment to prevent copying
        EngineModel(const EngineModel&) = delete;
        EngineModel& operator=(const EngineModel&) = delete;

        // Sets the bone matrices (used by the animation system)
        void setBoneMatrices(const std::vector<glm::mat4>& boneTransforms);

        // Binds the model's vertex and index buffers to the command buffer
        void bind(VkCommandBuffer commandBuffer);
        
        // Issues a draw call for the model
        void draw(VkCommandBuffer commandBuffer);

        // Returns the descriptor set for the model's textures
        VkDescriptorSet getTextureDescriptorSet() const { return textureDescriptorSet; }
        
        // Returns the descriptor set for the model's bone matrices
        VkDescriptorSet getBonesDescriptorSet() const { return bonesDescriptorSet; }

        // Texture samplers used by the model
        std::vector<std::shared_ptr<EngineTexture>> textures;

        // Bone transformation matrices (updated each frame)
        std::vector<glm::mat4> boneMatrices;

        // Current animation index (-1 if no animations)
        int currentAnimationIndex = -1;
        
        // Current time within the animation
        float currentTime = 0.0f;

        // Updates the animation state based on deltaTime
        void updateAnimation(float deltaTime);

        // Loads animations from a file
        void loadAnimationsFromFile(const std::string& animationFilePath);

        // Updates the push constants with scaling applied
        void UpdatePushConstantsWithScaling(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, const EngineTransformComponent& transform);

    private:
        // Internal storage for vertices and indices
        std::vector<Vertex> vertices{};
        std::vector<uint32_t> indices{};

        // Vulkan buffers for vertices, indices, and bones
        std::unique_ptr<EngineBuffer> boneBuffer;
        std::unique_ptr<EngineBuffer> vertexBuffer;
        std::unique_ptr<EngineBuffer> indexBuffer;

        // Vertex count for rendering
        uint32_t vertexCount = 0;
        
        // Index count and flag
        uint32_t indexCount = 0;
        bool hasIndexBuffer = false;

        // Animation data
        std::vector<EngineAnimation> animations;

        // Mapping from bone names to bone indices
        std::unordered_map<std::string, uint32_t> boneMapping;
        
        // Information about each bone
        std::vector<BoneInfo> boneInfos;

        // Global inverse transformation matrix (model space to world space)
        glm::mat4 globalInverseTransform;

        // Root node of the Assimp scene graph
        aiNode* rootNode = nullptr;

        // Pointer to the Assimp scene
        const aiScene* scene = nullptr;

        // Assimp importer instance
        std::unique_ptr<Assimp::Importer> importer;

        // Descriptor sets for textures and bones
        VkDescriptorSet textureDescriptorSet;
        VkDescriptorSet bonesDescriptorSet;

        // Creates Vulkan buffers for vertex data
        void createVertexBuffer(const std::vector<Vertex>& vertices);
        
        // Creates Vulkan buffers for index data
        void createIndexBuffer(const std::vector<uint32_t>& indices);
        
        // Loads a model from a file
        void loadModel(const std::string& filePath, const std::string& textureBaseDir = "");
        
        // Processes all animations in the scene
        void processAnimations(const aiScene* scene);
        
        // Collects bone information used in animations
        void collectAnimationBones(const aiScene* scene);
        
        // Recursively processes nodes in the scene graph
        void processNode(aiNode* node, const aiScene* scene, const std::string& textureBaseDir);
        
        // Processes a single mesh
        void processMesh(aiMesh* mesh, const aiScene* scene, const std::string& textureBaseDir);
        
        // Loads textures from a material
        void loadMaterialTextures(aiMaterial* material, aiTextureType type, const std::string& textureBaseDir);
        
        // Adds bone data to a vertex
        void addBoneData(Vertex& vertex, uint32_t boneID, float weight);

        // Creates descriptor sets for textures and bones
        void createDescriptorSets();

        // Helper functions to find keyframes (now returning indices)
        size_t findPositionKey(const std::vector<KeyPosition>& positions, float time) const;
        size_t findRotationKey(const std::vector<KeyRotation>& rotations, float time) const;
        size_t findScalingKey(const std::vector<KeyScaling>& scalings, float time) const;

        // Traverses the node hierarchy and applies bone transformations
        void traverseNodeHierarchy(aiNode* node, const glm::mat4& parentTransform, const EngineAnimation& currentAnim);
        
        // Interpolation helper functions
        glm::mat4 interpolatePosition(const std::vector<KeyPosition>& positions, float time) const;
        glm::mat4 interpolateRotation(const std::vector<KeyRotation>& rotations, float time) const;
        glm::mat4 interpolateScaling(const std::vector<KeyScaling>& scalings, float time) const;

        glm::mat4 coordinateFix = glm::mat4(1.0f);

        const aiNode* findFinalBoneNode(const aiNode* node);

        glm::mat4 convertMatrix(const aiMatrix4x4 &m) {
            // Assimp's aiMatrix4x4 is row-major; GLM is column-major. We must transpose.
            glm::mat4 matrix(
                m.a1, m.a2, m.a3, m.a4,
                m.b1, m.b2, m.b3, m.b4,
                m.c1, m.c2, m.c3, m.c4,
                m.d1, m.d2, m.d3, m.d4
            );
            return glm::transpose(matrix);
        }
    };
}
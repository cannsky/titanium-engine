#pragma once

#include <string>
#include <iostream>
#include "EngineVulkanManager.hpp"
#include "EngineSwapChain.hpp"
#include "EngineRenderSystem.hpp"
#include "EngineStaticRenderSystem.hpp"
#include "EngineFrameManager.hpp"
#include "EngineRenderer.hpp"
#include "EngineCamera.hpp"
#include "EngineModel.hpp"
#include "EngineInputManager.hpp"
#include "EngineLoopManager.hpp"
#include "EngineDebugManager.hpp"
#include "EngineFrameInfo.hpp"
#include "EngineDescriptors.hpp"
#include "EditorUIManager.hpp"
#include "EngineBuffer.hpp"
#include <memory>
#include <vector>
#include <array>
#include <numeric>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace TulparEngine {

    class EngineGraphicsManager {
    public:
        // Singleton access method for EngineGraphicsManager
        static EngineGraphicsManager& GetInstance() {
            static EngineGraphicsManager instance;
            return instance;
        }

        // Initialize all systems
        void Init();
        // Run the main engine loop
        void run();
        // Cleanup all systems
        void cleanup();

        EngineDescriptorPool& getGlobalPool() { return *globalPool; }
        EngineRenderer &getRenderer() { return renderer; }

        // Deleting copy constructor and assignment operator to enforce singleton
        EngineGraphicsManager(const EngineGraphicsManager&) = delete;
        void operator=(const EngineGraphicsManager&) = delete;
        
        void setLightIntensity(float intensity);

    private:
        EngineGraphicsManager();
        ~EngineGraphicsManager();

        void loadGameObjects();
        void createCamera();
        void generateEntity(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const std::string name, const std::string& modelPath);

        std::shared_ptr<EngineGameObject> viewerObject;
        std::vector<std::unique_ptr<EngineBuffer>> uniformBuffers;
        EngineRenderer renderer;
        EngineCamera camera;
        std::unique_ptr<EngineDescriptorPool> globalPool{};
        std::unique_ptr<EngineRenderSystem> renderSystem;
        std::unique_ptr<EngineStaticRenderSystem> staticRenderSystem;
        std::vector<VkDescriptorSet> globalDescriptorSets;
    };
}
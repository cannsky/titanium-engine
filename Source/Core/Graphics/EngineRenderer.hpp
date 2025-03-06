#pragma once

#include "EngineSwapChain.hpp"
#include "EngineVulkanManager.hpp"
#include "EngineWindowManager.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <memory>
#include <vector>
#include <cassert>

namespace TulparEngine {
    class EngineRenderer {
    public:
        EngineRenderer();
        ~EngineRenderer();

        EngineRenderer(const EngineRenderer&) = delete;
        EngineRenderer &operator=(const EngineRenderer&) = delete;

        VkRenderPass getSwapChainRenderPass() { return swapChain->getRenderPass(); }
        float getAspectRatio() { return swapChain->extentAspectRatio(); }
        bool isFrameInProgress() { return isFrameStarted; }

        VkCommandBuffer getCurrentCommandBuffer() {
            assert(isFrameStarted && "Cannot get command buffer when frame not in progress.");
            return commandBuffers[currentFrameIndex]; 
        }

        int getFrameIndex() {
            assert(isFrameStarted && "Cannot get frame index when frame not in progress.");
            return currentFrameIndex;
        }

        VkCommandBuffer beginFrame();
        void endFrame();
        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
        void endSwapChainRenderPass(VkCommandBuffer commandBuffer);
    private:
        void createCommandBuffers();
        void freeCommandBuffers();
        void drawFrame();
        void recreateSwapChain();

        std::unique_ptr<EngineSwapChain> swapChain;
        std::vector<VkCommandBuffer> commandBuffers;

        uint32_t currentImageIndex;
        int currentFrameIndex;
        bool isFrameStarted;
    };
}
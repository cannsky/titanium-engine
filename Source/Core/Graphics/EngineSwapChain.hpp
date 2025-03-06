#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include "EngineVulkanManager.hpp"
#include "EngineWindowManager.hpp"
#include <memory>

namespace TulparEngine {
    class EngineSwapChain {
    public:
        static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

        EngineSwapChain();
        EngineSwapChain(std::shared_ptr<EngineSwapChain> previousSwapChain);
        ~EngineSwapChain();

        void Init();

        VkFramebuffer getFrameBuffer(int index) { return swapChainFramebuffers[index]; }
        VkRenderPass getRenderPass() { return renderPass; }
        VkImageView getImageView(int index) { return swapChainImageViews[index]; }
        size_t imageCount() { return swapChainImages.size(); }
        VkFormat getSwapChainImageFormat() { return swapChainImageFormat; }
        VkExtent2D getSwapChainExtent() { return swapChainExtent; }
        uint32_t getWidth() { return swapChainExtent.width; }
        uint32_t getHeight() { return swapChainExtent.height; }

        static std::unique_ptr<EngineSwapChain> create();
        static std::unique_ptr<EngineSwapChain> create(std::shared_ptr<EngineSwapChain> previousSwapChain);

        float extentAspectRatio() {
            return static_cast<float>(swapChainExtent.width) / static_cast<float>(swapChainExtent.height);
        }
        VkFormat findDepthFormat();

        VkResult acquireNextImage(uint32_t *imageIndex);
        VkResult submitCommandBuffers(const VkCommandBuffer *buffers, uint32_t *imageIndex);

        bool compareSwapFormats(const EngineSwapChain &swapChain) {
            return swapChainImageFormat == swapChain.swapChainImageFormat && swapChainDepthFormat == swapChain.swapChainDepthFormat;
        }
    private:
        void createSwapChain();
        void createImageViews();
        void createDepthResources();
        void createRenderPass();
        void createFramebuffers();
        void createSyncObjects();

        // Helper functions
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

        VkFormat swapChainImageFormat;
        VkFormat swapChainDepthFormat;
        VkExtent2D swapChainExtent;

        std::vector<VkFramebuffer> swapChainFramebuffers;
        VkRenderPass renderPass;

        std::vector<VkImage> depthImages;
        std::vector<VkDeviceMemory> depthImageMemorys;
        std::vector<VkImageView> depthImageViews;
        std::vector<VkImage> swapChainImages;
        std::vector<VkImageView> swapChainImageViews;

        VkExtent2D windowExtent;

        VkSwapchainKHR swapChain;
        std::shared_ptr<EngineSwapChain> previousSwapChain;

        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;
        std::vector<VkFence> imagesInFlight;
        size_t currentFrame = 0;
    };
}
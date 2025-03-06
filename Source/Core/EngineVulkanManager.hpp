#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include <string>
#include <iostream>
#include <vector>
#include <set>
#include "EngineDebugManager.hpp"
#include "EngineWindowManager.hpp"
#ifdef QT_ENABLED
#include <QImage>
#endif

namespace TulparEngine {
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };
    struct QueueFamilyIndices {
        uint32_t graphicsFamily;
        uint32_t presentFamily;
        bool graphicsFamilyHasValue = false;
        bool presentFamilyHasValue = false;
        bool isComplete() { return graphicsFamilyHasValue && presentFamilyHasValue; }
    };
    class EngineVulkanManager {
    public:
        #ifdef NDEBUG
            const bool enableValidationLayers = false;
        #else
            const bool enableValidationLayers = true;
        #endif
        
        // Singleton access method for EngineVulkanManager
        static EngineVulkanManager& GetInstance() {
            static EngineVulkanManager instance;
            return instance;
        }

        // Initialize all systems
        void Init();

        // Run the main engine loop
        void run();

        // Cleanup all systems
        void cleanup();

        VkInstance getVulkanInstance() const { return instance; }
        VkDevice getDevice() const { return device; }
        VkPhysicalDevice getPhysicalDevice() const { return physicalDevice; }
        VkSwapchainKHR getSwapChain() const { return swapChain; }
        VkQueue getPresentQueue() const { return presentQueue; }
        VkSurfaceKHR getSurface() const { return surface; }
        QueueFamilyIndices findPhysicalQueueFamilies() { return findQueueFamilies(physicalDevice); }
        VkQueue getGraphicsQueue() const { return graphicsQueue; }
        VkCommandPool getCommandPool() const { return commandPool; }
        SwapChainSupportDetails getSwapChainSupport() { return querySwapChainSupport(physicalDevice); }
        void createImageWithInfo(const VkImageCreateInfo &imageInfo, VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory &imageMemory);
        VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory);
        VkCommandBuffer beginSingleTimeCommands();
        void endSingleTimeCommands(VkCommandBuffer commandBuffer);
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount);

        void deviceWaitIdle();

        VkPhysicalDeviceProperties getPhysicalDeviceProperties() { return physicalDeviceProperties; }

        // Deleting copy constructor and assignment operator to enforce singleton
        EngineVulkanManager(const EngineVulkanManager&) = delete;
        void operator=(const EngineVulkanManager&) = delete;

        // temp

        VkDescriptorSetLayout getTextureDescriptorSetLayout() const { return textureDescriptorSetLayout; }
        VkDescriptorPool getTextureDescriptorPool() const { return textureDescriptorPool; }
        VkDescriptorSetLayout getBonesDescriptorSetLayout() { return bonesDescriptorSetLayout; }
        VkDescriptorPool getBonesDescriptorPool() { return bonesDescriptorPool; }
        void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
        #ifdef QT_ENABLED
        VkImage CreateVulkanImageFromQImage(const QImage& img);
        #endif
        VkImageView createImageView(VkImage image, VkFormat format);
        void createTextureSampler();
        void allocateTextureDescriptorSet(VkDescriptorSet &descriptorSet);
        void updateTextureDescriptorSet(VkDescriptorSet descriptorSet, VkImageView imageView);

    private:
        EngineVulkanManager();  // Private constructor for singleton
        ~EngineVulkanManager();

        VkInstance instance;
        VkDevice device;
        VkQueue graphicsQueue;
        VkQueue presentQueue;
        VkCommandPool commandPool;
        VkRenderPass renderPass;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkSurfaceKHR surface;
        VkPhysicalDeviceFeatures deviceFeatures{};
        VkPhysicalDeviceProperties physicalDeviceProperties;
        VkDebugUtilsMessengerEXT debugMessenger;
        VkSampler textureSampler;

        VkSwapchainKHR swapChain;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
        std::vector<VkImage> swapChainImages;
        std::vector<VkImageView> swapChainImageViews;
        std::vector<VkFramebuffer> swapChainFramebuffers;
        
        const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
        const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

        // Helper methods for Vulkan initialization and cleanup
        void createInstance();
        void setupDebugMessenger();
        void createSurface();
        void pickPhysicalDevice();
        void createLogicalDevice();
        void createSwapChain();
        void createSwapChainImageViews();
        void createCommandPool();
        void createRenderPass();
        void createFramebuffers();
        void cleanupSwapChain();
        void hasGlfwRequiredInstanceExtensions();
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        bool isDeviceSuitable(VkPhysicalDevice device);
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);
        bool checkValidationLayerSupport();
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
        std::vector<const char*> getRequiredExtensions();
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

        // temp

        VkDescriptorSetLayout textureDescriptorSetLayout;
        VkDescriptorPool textureDescriptorPool;
        VkDescriptorSetLayout bonesDescriptorSetLayout;
        VkDescriptorPool bonesDescriptorPool;

        void createTextureDescriptorSetLayout();
        void createTextureDescriptorPool();
        void createBonesDescriptorSetLayout();
        void createBonesDescriptorPool();
    };
}
#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <cstdint>
#include <stdexcept>
#include "EngineDebugManager.hpp"

namespace TulparEngine {

    class EngineWindowManager {
    public:
        // Singleton access method
        static EngineWindowManager& GetInstance() {
            static EngineWindowManager instance;
            return instance;
        }

        void Init();
        // Initialize the window
        void initWindow(uint32_t width, uint32_t height, const char* title);
        // Poll for window events
        void pollEvents();
        // Cleanup the window
        void cleanup();
        // Get the window instance
        static GLFWwindow& getWindow();
        // Get the window pointer
        static GLFWwindow* getWindowPointer() { return GetInstance().window; }
        static void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
        static VkExtent2D getExtent() { return { static_cast<uint32_t>(800), static_cast<uint32_t>(600) }; }
        bool wasWindowResized() { return framebufferResized; }
        void resetWindowResizedFlag() { framebufferResized = false; }
        
    private:
        // Private constructor for singleton
        EngineWindowManager();
        ~EngineWindowManager();

        static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

        int width;
        int height;
        bool framebufferResized = false;
        
        // Delete copy and assignment operators to enforce singleton
        EngineWindowManager(const EngineWindowManager&) = delete;
        void operator=(const EngineWindowManager&) = delete;
        // Window handle
        GLFWwindow* window = nullptr;
    };
}

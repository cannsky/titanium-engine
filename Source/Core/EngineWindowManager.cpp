#include "EngineWindowManager.hpp"

namespace TulparEngine {

    EngineWindowManager::EngineWindowManager() {}

    EngineWindowManager::~EngineWindowManager() {
        cleanup();
    }

    void EngineWindowManager::Init() {
        // Initialize the window
        initWindow(1280, 720, "Tulpar Engine");
    }

    void EngineWindowManager::initWindow(uint32_t width, uint32_t height, const char* title) {
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);  // No OpenGL context
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);    // Non-resizable window

        // Create the window
        window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
        // Check if the window was created successfully
        if (!window) {
            throw std::runtime_error("Failed to create GLFW window");
        }

        EngineDebugManager::log("Window created successfully", DebugCategory::WindowManager);
    }

    void EngineWindowManager::pollEvents() {
        glfwPollEvents();
    }

    void EngineWindowManager::cleanup() {
        if (window) {
            glfwDestroyWindow(window);
            window = nullptr;
        }
        glfwTerminate();
    }

    GLFWwindow& EngineWindowManager::getWindow() {
        return *GetInstance().window;
    }

    void EngineWindowManager::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
        if (glfwCreateWindowSurface(instance, GetInstance().window, nullptr, surface) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create window surface");
        }
        EngineDebugManager::log("Surface created sucessfully", DebugCategory::WindowManager);
    }

    void EngineWindowManager::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
        auto instance = reinterpret_cast<EngineWindowManager *>(glfwGetWindowUserPointer(window));
        instance->framebufferResized = true;
        instance->width = width;
        instance->height = height;
    }
}

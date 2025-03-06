#pragma once

#include <string>
#include <iostream>
#include <cassert>
#include <limits>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include "EngineGameObject.hpp"
#include "EngineWindowManager.hpp"

#include "EditorAssistantChatUI.hpp"

namespace TulparEngine {
    class EngineInputManager {
    public:

        struct KeyMappings {
            int moveLeft = GLFW_KEY_A;
            int moveRight = GLFW_KEY_D;
            int moveForward = GLFW_KEY_W;
            int moveBackward = GLFW_KEY_S;
            int moveUp = GLFW_KEY_E;
            int moveDown = GLFW_KEY_Q;
            int lookLeft = GLFW_KEY_LEFT;
            int lookRight = GLFW_KEY_RIGHT;
            int lookUp = GLFW_KEY_UP;
            int lookDown = GLFW_KEY_DOWN;
        };

        void moveInPlaneXZ(float deltaTime, EngineGameObject& gameObject);

        KeyMappings keys;
        float moveSpeed = 3.0f;
        float lookSpeed = 1.5f;

        // Singleton access method for EngineInputManager
        static EngineInputManager& GetInstance() {
            static EngineInputManager instance;
            return instance;
        }

        // Initialize all systems
        void Init();

        // Run the main engine loop
        void run();

        // Cleanup all systems
        void cleanup();

        // Get the current frame
        static int frame;

        // Deleting copy constructor and assignment operator to enforce singleton
        EngineInputManager(const EngineInputManager&) = delete;
        void operator=(const EngineInputManager&) = delete;
    private:
        EngineInputManager();  // Private constructor for singleton
        ~EngineInputManager();
    };
}
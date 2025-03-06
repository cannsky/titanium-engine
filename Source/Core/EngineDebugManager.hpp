#pragma once

#include <string>
#include <iostream>
#include <glm/glm.hpp>

namespace TulparEngine {
    enum class DebugCategory {
        WindowManager,
        GraphicsPipeline,
        Renderer,
        VulkanManager,
        FrameManager,
        GraphicsManager,
        Error,
        Model,
        EntityManager,
        UserDebug,
        SceneManager,
        Scene,
        PhysicsCollisions,
        Animation,
        Assistant,
        BrowserManager,
        Physics,
        AI
    };

    class EngineDebugManager {
    public:
        // Singleton access method for EngineFrameManager
        static EngineDebugManager& GetInstance() {
            static EngineDebugManager instance;
            return instance;
        }

        // Initialize all systems
        void Init();

        // Run the main engine loop
        void run();

        // Cleanup all systems
        void cleanup();

        static void log(const std::string& message, DebugCategory category);

        // Log Engine Error
        static void logEngineError(const std::string& message);

        // Log matrix
        static void logMatrix(const std::string& message, const glm::mat4& matrix);

        // Deleting copy constructor and assignment operator to enforce singleton
        EngineDebugManager(const EngineDebugManager&) = delete;
        void operator=(const EngineDebugManager&) = delete;

        bool isGraphicsPipelineDebug = true;
        bool isRendererDebug = true;
        bool isVulkanManagerDebug = true;
        bool isFrameManagerDebug = true;
        bool isWindowManagerDebug = true;
        bool isErrorDebug = true;
        bool isModelDebug = false;
        bool isUserDebug = true;
        bool isEntityManagerDebug = false;
        bool isSceneManagerDebug = true;
        bool isSceneDebug = false;
        bool isPhysicsCollisionsDebug = true;
        bool isAnimationDebug = true;
        bool isAssistantDebug = true;
        bool isBrowserManagerDebug = true;
        bool isPhysicsDebug = true;
        bool isAIDebug = true;
    private:
        EngineDebugManager();  // Private constructor for singleton
        ~EngineDebugManager();
    };
}
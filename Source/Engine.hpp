#pragma once

#include <GLFW/glfw3.h>
#include "EngineWindowManager.hpp"
#include "EngineDebugManager.hpp"
#include "EngineFrameManager.hpp"
#include "EngineVulkanManager.hpp"
#include "EngineFileManager.hpp"
#include "EngineGraphicsManager.hpp"
#include "EngineScriptManager.hpp"
#include "EngineLoopManager.hpp"
#include "EngineInputManager.hpp"
#include "EngineEntityManager.hpp"
#include "EngineSceneManager.hpp"
#include "EngineDestructionManager.hpp"

#include "Editor.hpp"
// #include "PhysicsEngine.hpp"
#include "EngineTools.hpp"

#include "GameManager.hpp"

// #define USE_QT

using namespace TulparEngine::Examples::SimpleShooter;
using namespace TulparEngine::EngineEditor;
using namespace TulparEngine::Tools;

namespace TulparEngine {
    class Engine {
    public:
        // Singleton access method for the Engine
        static Engine& GetInstance() {
            static Engine instance;
            return instance;
        }

        // Initialize all systems
        void Init();

        // Start all systems
        void Start();

        // Run the main engine loop
        void run();

        // Cleanup all systems
        void cleanup();

        // Deleting copy constructor and assignment operator to enforce singleton
        Engine(const Engine&) = delete;
        void operator=(const Engine&) = delete;

        // Boolean for debug mode
        bool isDebugModeEnabled = true;
        bool isEditorEnabled = true;
        bool isPhysicsEnabled = true;
        bool isToolsEnabled = true;
        bool isTestModeEnabled = true;
    private:
        Engine();  // Private constructor for singleton
        ~Engine();
    };
}
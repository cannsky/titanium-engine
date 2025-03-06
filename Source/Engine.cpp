#include "Engine.hpp"

namespace TulparEngine {

    // Private constructor
    Engine::Engine() {}

    // Destructor
    Engine::~Engine() {}

    /*
        Initialize all systems

        Dependencies:
            - EngineDebugManager: Essential and depended on by all other systems
            - EngineFileManager Essential and depended on by all other systems
            - EngineWindowManager: Depends on EngineDebugManager, EngineFileManager
            - EngineVulkanManager: Depends on EngineDebugManager, EngineWindowManager

    */
    void Engine::Init() {
        EngineDebugManager::GetInstance().Init();
        EngineFileManager::GetInstance().Init();
        EngineWindowManager::GetInstance().Init();
        EngineVulkanManager::GetInstance().Init();
        EngineGraphicsManager::GetInstance().Init();
        EngineFrameManager::GetInstance().Init();
        EngineScriptManager::GetInstance().Init();
        EngineInputManager::GetInstance().Init();
        EngineEntityManager::GetInstance().Init();
        EngineSceneManager::GetInstance().Init();
        EngineLoopManager::GetInstance().Init();
        EngineDestructionManager::GetInstance().Init();

        #ifdef USE_QT
            isToolsEnabled = true;
        #else
            isToolsEnabled = false;
        #endif

        //if (isPhysicsEnabled) PhysicsEngine::GetInstance().Init();
        
        if (isEditorEnabled) Editor::GetInstance().Init();

        if (isToolsEnabled) EngineTools::GetInstance().Init();

        if (isTestModeEnabled) GameManager::GetInstance().Init();
    }

    // Run the main engine loop
    void Engine::run() {
        try {
            while (!glfwWindowShouldClose(&EngineWindowManager::GetInstance().getWindow())) {
                EngineLoopManager::GetInstance().run();
                #ifdef USE_QT
                EngineBrowserManager::GetInstance().Run();
                #endif
                EngineWindowManager::GetInstance().pollEvents();
                EngineTools::GetInstance().Run();
                if (isTestModeEnabled) GameManager::GetInstance().Run();
            }
        } catch (const std::exception& e) { EngineDebugManager::log(e.what(), DebugCategory::Error); }

        // Wait for the device to finish before cleanup
        EngineVulkanManager::GetInstance().deviceWaitIdle();
    }

    // Cleanup all systems
    void Engine::cleanup() {
        
    }

}

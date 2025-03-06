#include "EngineLoopManager.hpp"

namespace TulparEngine {

    int EngineLoopManager::frame = 0;

    EngineLoopManager::EngineLoopManager() {}
    EngineLoopManager::~EngineLoopManager() { cleanup(); }

    void EngineLoopManager::Init() {
        newTime = std::chrono::high_resolution_clock::now();
    }

    void EngineLoopManager::run() {
        // 1. Compute delta time
        calculateDeltaTime();

        // 2. Run your various managers
        EngineEntityManager::GetInstance().run();
        EngineFrameManager::GetInstance().run();
        EngineSceneManager::GetInstance().run();
        EngineAIManager::GetInstance().Run();
        TulparEngine::EngineEditor::Editor::GetInstance().run();

        // 3. Increase frame index (this is your "cpu iteration" frame)
        frame++;

        // 4. Defer destruction check
        //EngineDestructionManager::GetInstance().Run();
    }

    void EngineLoopManager::calculateDeltaTime() {
        lastTime = newTime;
        newTime = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - lastTime).count();
    }

    void EngineLoopManager::cleanup() {
        // ...
    }
}
#include "EngineFrameManager.hpp"

namespace TulparEngine {
    
    int EngineFrameManager::frame = 0;

    EngineFrameManager::EngineFrameManager() {}

    EngineFrameManager::~EngineFrameManager() { cleanup(); }

    void EngineFrameManager::Init() {

    }

    // Run the main engine loop
    void EngineFrameManager::run() {
        // Start the frame
        newFrame();
        // Poll events
        EngineWindowManager::GetInstance().pollEvents();
        // Run the graphics manager
        EngineGraphicsManager::GetInstance().run();
    }

    void EngineFrameManager::cleanup() {

    }

    void EngineFrameManager::newFrame() {
        EngineFrameManager::frame = (EngineFrameManager::frame + 1) % 1000;
    }
}
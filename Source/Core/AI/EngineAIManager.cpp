#include "EngineAIManager.hpp"

namespace TulparEngine {
    EngineAIManager::EngineAIManager() {
        
    }

    EngineAIManager::~EngineAIManager() {

    }

    void EngineAIManager::Init() {
        EngineAISystemManager::GetInstance();
    }

    void EngineAIManager::Start() {
        // Start EngineAIManager
    }

    void EngineAIManager::Run() {
        EngineAISystemManager::GetInstance().Update();
    }

    void EngineAIManager::Cleanup() {
        // Cleanup EngineAIManager
    }
}
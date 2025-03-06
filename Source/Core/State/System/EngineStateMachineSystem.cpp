#include "EngineStateMachineSystem.hpp"
#include "EngineDebugManager.hpp"

namespace TulparEngine {
    void EngineStateMachineSystem::UpdateBatch(EngineArchetype& archetype, size_t startIdx, size_t endIdx) {
        float deltaTime = EngineLoopManager::getDeltaTime();

        for (size_t i = startIdx; i < endIdx; ++i) {
            auto stateMachineComponent = std::static_pointer_cast<EngineStateMachineComponent>(
                archetype.GetComponent(i, typeid(EngineStateMachineComponent)));

            
        }
    }
}
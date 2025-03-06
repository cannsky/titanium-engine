#include "EngineAnimationSystem.hpp"

namespace TulparEngine {
    void EngineAnimationSystem::UpdateBatch(EngineArchetype& archetype, size_t startIdx, size_t endIdx) {
        float deltaTime = EngineLoopManager::getDeltaTime();

        for (size_t i = startIdx; i < endIdx; ++i) {
            auto animatorComponent = std::static_pointer_cast<EngineAnimatorComponent>(
                archetype.GetComponent(i, typeid(EngineAnimatorComponent)));
            auto modelComponent = std::static_pointer_cast<EngineModelComponent>(
                archetype.GetComponent(i, typeid(EngineModelComponent)));

            modelComponent->model->updateAnimation(deltaTime);
        }
    }
}
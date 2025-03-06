#pragma once

#include "EngineSystem.hpp"
#include "EngineAnimatorComponent.hpp"
#include "EngineModelComponent.hpp"
#include "EngineTransformComponent.hpp"
#include "EngineSystemRegistration.hpp"
#include "EngineLoopManager.hpp"

namespace TulparEngine {
    class EngineAnimationSystem : public EngineSystem {
    public:
        void UpdateBatch(EngineArchetype& archetype, size_t startIdx, size_t endIdx) override;

        const std::vector<ComponentType>& GetComponentTypes() const override {
            static std::vector<ComponentType> types = {
                typeid(EngineAnimatorComponent),
                typeid(EngineModelComponent)
            };
            return types;
        }
    };

    REGISTER_SYSTEM(EngineAnimationSystem)
}
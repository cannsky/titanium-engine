#pragma once

#include "EngineSystem.hpp"
#include "EngineStateMachineComponent.hpp"
#include "EngineLoopManager.hpp"
#include "EngineSystemRegistration.hpp"

namespace TulparEngine {
    class EngineStateMachineSystem : public EngineSystem {
    public:
        void UpdateBatch(EngineArchetype& archetype, size_t startIdx, size_t endIdx) override;

        const std::vector<ComponentType>& GetComponentTypes() const override {
            static std::vector<ComponentType> types = {
                typeid(EngineStateMachineComponent)
            };
            return types;
        }
    };

    REGISTER_SYSTEM(EngineStateMachineSystem)
}
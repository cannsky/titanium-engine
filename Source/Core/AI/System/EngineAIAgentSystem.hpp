#pragma once

#include "EngineSystem.hpp"
#include "EngineAIAgentComponent.hpp"
#include "EngineLoopManager.hpp"
#include "EngineAISystemRegistration.hpp"

namespace TulparEngine {
    class EngineAIAgentSystem : public EngineSystem {
    public:
        void UpdateBatch(EngineArchetype& archetype, size_t startIdx, size_t endIdx) override;

        const std::vector<ComponentType>& GetComponentTypes() const override {
            static std::vector<ComponentType> types = {
                typeid(EngineAIAgentComponent)
            };
            return types;
        }
    };
}
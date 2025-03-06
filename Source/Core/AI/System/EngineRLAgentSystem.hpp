#pragma once

#include "EngineSystem.hpp"
#include "EngineRLAgentComponent.hpp"
#include "PlayerAttackComponent.hpp"
#include "PlayerMoveComponent.hpp"
#include "PlayerRewardComponent.hpp"
#include "PlayerRotateComponent.hpp"
#include "PlayerStatsComponent.hpp"
#include "PlayerInputComponent.hpp"
#include "EngineLoopManager.hpp"
#include "EngineAISystemRegistration.hpp"

using namespace TulparEngine::Examples::SimpleShooter;

namespace TulparEngine {
    class EngineRLAgentSystem : public EngineSystem {
    public:
        void UpdateBatch(EngineArchetype& archetype, size_t startIdx, size_t endIdx) override;

        const std::vector<ComponentType>& GetComponentTypes() const override {
            static std::vector<ComponentType> types = {
                typeid(EngineRLAgentComponent),
                typeid(EngineTransformComponent),
                typeid(PlayerMoveComponent),
                typeid(PlayerRotateComponent),
                typeid(PlayerStatsComponent),
                typeid(PlayerAttackComponent),
                typeid(PlayerInputComponent)
            };
            return types;
        }
    };
}
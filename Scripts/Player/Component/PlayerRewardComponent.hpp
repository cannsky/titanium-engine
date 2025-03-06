#pragma once

#include "EngineEntity.hpp"
#include "EngineComponent.hpp"
#include "EngineComponentRegistration.hpp"

namespace TulparEngine::Examples::SimpleShooter {
    // PlayerRewardComponent derived from EngineComponent
    class PlayerRewardComponent : public EngineComponent {
    public:
        virtual ComponentType GetType() const override {
            return typeid(PlayerRewardComponent);
        }

        float reward = 0.0f;
        float hitReward = 1.0f;
        float takeDownReward = 10.0f;
        float hitPenalty = 0.5f;
        float takeDownPenalty = 8.0f;
    };

    // Register PlayerRewardComponent
    //REGISTER_COMPONENT(PlayerRewardComponent, "PlayerRewardComponent");
}
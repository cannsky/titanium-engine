#pragma once

#include "EngineComponent.hpp"
#include "EngineComponentRegistration.hpp"

namespace TulparEngine::Examples::SimpleShooter {
    // PlayerStatsComponent derived from EngineComponent
    class PlayerStatsComponent : public EngineComponent {
    public:
        virtual ComponentType GetType() const override {
            return typeid(PlayerStatsComponent);
        }

        float health = 100.0f;
        float damage = 50.0f;
        bool isDead = false;
        int hitCounter = 0;
        int givenHitCounter = 0;
    };

    // Register PlayerStatsComponent
    //REGISTER_COMPONENT(PlayerStatsComponent, "PlayerStatsComponent");
}
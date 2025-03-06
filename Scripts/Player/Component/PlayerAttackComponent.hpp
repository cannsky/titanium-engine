#pragma once

#include "EngineComponent.hpp"
#include "EngineComponentRegistration.hpp"

namespace TulparEngine::Examples::SimpleShooter {
    // PlayerAttackComponent derived from EngineComponent
    class PlayerAttackComponent : public EngineComponent {
    public:
        virtual ComponentType GetType() const override {
            return typeid(PlayerAttackComponent);
        }

        bool isAttackingCooldown = false;
        bool isAttacked = false;
        float attackCooldown = 5.0f;
        float currentAttackCooldown = 0.0f;
    };

    // Register PlayerAttackComponent
    //REGISTER_COMPONENT(PlayerAttackComponent, "PlayerAttackComponent");
}
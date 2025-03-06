#pragma once

#include "EngineSystem.hpp"
#include "EngineSystemRegistration.hpp"
#include "EngineEntityManager.hpp"
#include "EngineDebugManager.hpp"
#include "EngineLoopManager.hpp"
#include "PlayerAttackComponent.hpp"
#include "PlayerInputComponent.hpp"

namespace TulparEngine::Examples::SimpleShooter {
    // PlayerMoveSystem derived from EngineSystem
    class PlayerAttackSystem : public EngineSystem {
    public:
        virtual void UpdateBatch(EngineArchetype& archetype, size_t startIdx, size_t endIdx) override {
            for (size_t i = startIdx; i < endIdx; ++i) {

                auto transformComponent = std::static_pointer_cast<EngineTransformComponent>(
                    archetype.GetComponent(i, typeid(EngineTransformComponent)));

                auto inputComponent = std::static_pointer_cast<PlayerInputComponent>(
                    archetype.GetComponent(i, typeid(PlayerInputComponent)));

                auto attackComponent = std::static_pointer_cast<PlayerAttackComponent>(
                    archetype.GetComponent(i, typeid(PlayerAttackComponent)));

                if (inputComponent->isAttacking && !attackComponent->isAttackingCooldown) {
                    // Start attack cooldown
                    attackComponent->isAttackingCooldown = true;
                    attackComponent->currentAttackCooldown = attackComponent->attackCooldown;

                    attackComponent->isAttacked = true;
                }

                if (attackComponent->isAttackingCooldown) {
                    attackComponent->currentAttackCooldown -= EngineLoopManager::GetInstance().getDeltaTime();
                    if (attackComponent->currentAttackCooldown <= 0) {
                        attackComponent->isAttackingCooldown = false;
                        attackComponent->currentAttackCooldown = 0;
                    }
                }
            }
        }

        virtual const std::vector<ComponentType>& GetComponentTypes() const override {
            static std::vector<ComponentType> types = {
                typeid(EngineTransformComponent),
                typeid(PlayerAttackComponent),
                typeid(PlayerInputComponent)
            };
            return types;
        }
    };

    //REGISTER_SYSTEM(PlayerAttackSystem);
}

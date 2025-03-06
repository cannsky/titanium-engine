#pragma once

#include "EngineSystem.hpp"
#include "EngineSystemRegistration.hpp"
#include "EngineEntityManager.hpp"
#include "EngineDebugManager.hpp"
#include "EngineLoopManager.hpp"
#include "PlayerTargetComponent.hpp"
#include "PlayerStatsComponent.hpp"
#include "PlayerRotateComponent.hpp"

namespace TulparEngine::Examples::SimpleShooter {
    // PlayerMoveSystem derived from EngineSystem
    class PlayerTargetSystem : public EngineSystem {
    public:
        virtual void UpdateBatch(EngineArchetype& archetype, size_t startIdx, size_t endIdx) override {
            for (size_t i = startIdx; i < endIdx; ++i) {

                auto transformComponent = std::static_pointer_cast<EngineTransformComponent>(
                    archetype.GetComponent(i, typeid(EngineTransformComponent)));

                auto targetComponent = std::static_pointer_cast<PlayerTargetComponent>(
                    archetype.GetComponent(i, typeid(PlayerTargetComponent)));

                // Get target entities transform, stats and rotation component
                auto targetTransformComponent = std::static_pointer_cast<EngineTransformComponent>(
                    EngineEntityManager::GetInstance().GetComponent(targetComponent->targetEntityID, typeid(EngineTransformComponent)));

                auto targetStatsComponent = std::static_pointer_cast<PlayerStatsComponent>(
                    EngineEntityManager::GetInstance().GetComponent(targetComponent->targetEntityID, typeid(PlayerStatsComponent)));

                auto targetRotateComponent = std::static_pointer_cast<PlayerRotateComponent>(
                    EngineEntityManager::GetInstance().GetComponent(targetComponent->targetEntityID, typeid(PlayerRotateComponent)));

                // Update target position
                targetComponent->targetX = targetTransformComponent->translation.x;
                targetComponent->targetZ = targetTransformComponent->translation.z;
                targetComponent->targetRotX = targetTransformComponent->rotation.x;
                targetComponent->targetHealth = targetStatsComponent->health;
            }
        }

        virtual const std::vector<ComponentType>& GetComponentTypes() const override {
            static std::vector<ComponentType> types = {
                typeid(EngineTransformComponent),
                typeid(PlayerTargetComponent)
            };
            return types;
        }
    };

    //REGISTER_SYSTEM(PlayerTargetSystem);
}

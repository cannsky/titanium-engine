#pragma once

#include "EngineSystem.hpp"
#include "EngineEntityManager.hpp"
#include "EngineDebugManager.hpp"
#include "EngineLoopManager.hpp"
#include "ProjectileStatsComponent.hpp"
#include "ProjectileTargetComponent.hpp"
#include "PlayerStatsComponent.hpp"

namespace TulparEngine::Examples::SimpleShooter {
    // ProjectileMoveSystem derived from EngineSystem
    class ProjetileHitSystem : public EngineSystem {
    public:
        virtual void UpdateBatch(EngineArchetype& archetype, size_t startIdx, size_t endIdx) override {
            for (size_t i = startIdx; i < endIdx; ++i) {
                auto transformComponent = std::static_pointer_cast<EngineTransformComponent>(
                    archetype.GetComponent(i, typeid(EngineTransformComponent)));

                auto targetComponent = std::static_pointer_cast<ProjectileTargetComponent>(
                    archetype.GetComponent(i, typeid(ProjectileTargetComponent)));

                auto statsComponent = std::static_pointer_cast<ProjectileStatsComponent>(
                    archetype.GetComponent(i, typeid(ProjectileStatsComponent)));
                
                if (targetComponent->targetEntityID > 0 && !statsComponent->isDestroyed) {
                    auto targetTransformComponent = EngineEntityManager::GetInstance().GetComponent<EngineTransformComponent>(targetComponent->targetEntityID);

                    if (targetTransformComponent) {
                        float distance = glm::distance(transformComponent->translation, targetTransformComponent->translation);
                        //EngineDebugManager::log("Distance: " + std::to_string(distance), DebugCategory::UserDebug);
                        if (distance < 1.18f) {
                            auto targetPlayerStatsComponent = EngineEntityManager::GetInstance().GetComponent<PlayerStatsComponent>(targetComponent->targetEntityID);
                            targetPlayerStatsComponent->health -= statsComponent->damage;
                            targetPlayerStatsComponent->hitCounter++;
                            auto ownerPlayerStatsComponent = EngineEntityManager::GetInstance().GetComponent<PlayerStatsComponent>(targetComponent->ownerEntityID);
                            ownerPlayerStatsComponent->givenHitCounter++;
                            EngineEntityManager::GetInstance().RemoveEntity(archetype.entities[i]);
                            statsComponent->isDestroyed = true;
                        }
                    }
                }
            }
        }

        virtual const std::vector<ComponentType>& GetComponentTypes() const override {
            static std::vector<ComponentType> types = {
                typeid(EngineTransformComponent),
                typeid(ProjectileTargetComponent),
                typeid(ProjectileStatsComponent)
            };
            return types;
        }
    };
}

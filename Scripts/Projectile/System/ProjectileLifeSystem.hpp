#pragma once

#include "EngineSystem.hpp"
#include "EngineEntityManager.hpp"
#include "EngineDebugManager.hpp"
#include "EngineLoopManager.hpp"
#include "ProjectileMoveComponent.hpp"
#include "ProjectileLifeComponent.hpp"

namespace TulparEngine::Examples::SimpleShooter {
    // ProjectileLifeSystem derived from EngineSystem
    class ProjectileLifeSystem : public EngineSystem {
    public:
        virtual void UpdateBatch(EngineArchetype& archetype, size_t startIdx, size_t endIdx) override {
            for (size_t i = startIdx; i < endIdx; ++i) {
                auto transformComponent = std::static_pointer_cast<EngineTransformComponent>(
                    archetype.GetComponent(i, typeid(EngineTransformComponent)));

                auto projectileLifeComponent = std::static_pointer_cast<ProjectileLifeComponent>(
                    archetype.GetComponent(i, typeid(ProjectileLifeComponent)));

                auto projectileStatsComponent = std::static_pointer_cast<ProjectileStatsComponent>(
                    archetype.GetComponent(i, typeid(ProjectileStatsComponent)));

                float newCurrentLifeTime = projectileLifeComponent->currentLifeTime + EngineLoopManager::GetInstance().getDeltaTime();

                if (newCurrentLifeTime >= projectileLifeComponent->lifeTime) {
                    // Now we can get the entity ID safely via:
                    auto entityID = archetype.GetEntityID(i);

                    // And then remove it from the entity manager
                    EngineEntityManager::GetInstance().RemoveEntity(entityID);
                } 
                else if (projectileStatsComponent->isDestroyed) {
                    // Now we can get the entity ID safely via:
                    auto entityID = archetype.GetEntityID(i);

                    // And then remove it from the entity manager
                    EngineEntityManager::GetInstance().RemoveEntity(entityID);
                }
                else {
                    projectileLifeComponent->currentLifeTime = newCurrentLifeTime;
                }
            }
        }

        virtual const std::vector<ComponentType>& GetComponentTypes() const override {
            static std::vector<ComponentType> types = {
                typeid(EngineTransformComponent),
                typeid(ProjectileLifeComponent),
                typeid(ProjectileStatsComponent)
            };
            return types;
        }
    };
}

#pragma once

#include "EngineSystem.hpp"
#include "EngineEntityManager.hpp"
#include "EngineDebugManager.hpp"
#include "EngineLoopManager.hpp"
#include "ProjectileMoveComponent.hpp"
#include "PlayerInputComponent.hpp"

namespace TulparEngine::Examples::SimpleShooter {
    // ProjectileMoveSystem derived from EngineSystem
    class ProjectileMoveSystem : public EngineSystem {
    public:
        virtual void UpdateBatch(EngineArchetype& archetype, size_t startIdx, size_t endIdx) override {
            for (size_t i = startIdx; i < endIdx; ++i) {
                auto transformComponent = std::static_pointer_cast<EngineTransformComponent>(
                    archetype.GetComponent(i, typeid(EngineTransformComponent)));

                auto moveComponent = std::static_pointer_cast<ProjectileMoveComponent>(
                    archetype.GetComponent(i, typeid(ProjectileMoveComponent)));

                transformComponent->translation.x += moveComponent->direction.x * moveComponent->speed * EngineLoopManager::getDeltaTime();
                transformComponent->translation.y += moveComponent->direction.y * moveComponent->speed * EngineLoopManager::getDeltaTime();
                transformComponent->translation.z += moveComponent->direction.z * moveComponent->speed * EngineLoopManager::getDeltaTime();
            }
        }

        virtual const std::vector<ComponentType>& GetComponentTypes() const override {
            static std::vector<ComponentType> types = {
                typeid(EngineTransformComponent),
                typeid(ProjectileMoveComponent)
            };
            return types;
        }
    };
}

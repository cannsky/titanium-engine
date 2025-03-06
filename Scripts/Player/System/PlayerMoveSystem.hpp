#pragma once

#include "EngineSystem.hpp"
#include "EngineSystemRegistration.hpp"
#include "EngineEntityManager.hpp"
#include "EngineDebugManager.hpp"
#include "EngineLoopManager.hpp"
#include "PlayerMoveComponent.hpp"
#include "PlayerInputComponent.hpp"

namespace TulparEngine::Examples::SimpleShooter {
    // PlayerMoveSystem derived from EngineSystem
    class PlayerMoveSystem : public EngineSystem {
    public:
        virtual void UpdateBatch(EngineArchetype& archetype, size_t startIdx, size_t endIdx) override {
            for (size_t i = startIdx; i < endIdx; ++i) {
                auto transformComponent = std::static_pointer_cast<EngineTransformComponent>(
                    archetype.GetComponent(i, typeid(EngineTransformComponent)));

                auto moveComponent = std::static_pointer_cast<PlayerMoveComponent>(
                    archetype.GetComponent(i, typeid(PlayerMoveComponent)));

                auto inputComponent = std::static_pointer_cast<PlayerInputComponent>(
                    archetype.GetComponent(i, typeid(PlayerInputComponent)));

                transformComponent->translation.x += inputComponent->movementInput.x * moveComponent->speed * EngineLoopManager::getDeltaTime();
                transformComponent->translation.y += inputComponent->movementInput.y * moveComponent->speed * EngineLoopManager::getDeltaTime();
                transformComponent->translation.z += inputComponent->movementInput.z * moveComponent->speed * EngineLoopManager::getDeltaTime();
            }
        }

        virtual const std::vector<ComponentType>& GetComponentTypes() const override {
            static std::vector<ComponentType> types = {
                typeid(EngineTransformComponent),
                typeid(PlayerMoveComponent),
                typeid(PlayerInputComponent)
            };
            return types;
        }
    };

    //REGISTER_SYSTEM(PlayerMoveSystem);
}

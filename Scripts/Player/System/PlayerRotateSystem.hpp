#pragma once

#include "EngineSystem.hpp"
#include "EngineSystemRegistration.hpp"
#include "EngineEntityManager.hpp"
#include "EngineDebugManager.hpp"
#include "EngineLoopManager.hpp"
#include "PlayerRotateComponent.hpp"
#include "PlayerInputComponent.hpp"

namespace TulparEngine::Examples::SimpleShooter {
    // PlayerRotateSystem derived from EngineSystem
    class PlayerRotateSystem : public EngineSystem {
    public:
        virtual void UpdateBatch(EngineArchetype& archetype, size_t startIdx, size_t endIdx) override {
            for (size_t i = startIdx; i < endIdx; ++i) {
                auto transformComponent = std::static_pointer_cast<EngineTransformComponent>(
                    archetype.GetComponent(i, typeid(EngineTransformComponent)));

                auto rotateComponent = std::static_pointer_cast<PlayerRotateComponent>(
                    archetype.GetComponent(i, typeid(PlayerRotateComponent)));

                auto inputComponent = std::static_pointer_cast<PlayerInputComponent>(
                    archetype.GetComponent(i, typeid(PlayerInputComponent)));

                transformComponent->rotation.x += inputComponent->rotationInput.x * rotateComponent->speed * EngineLoopManager::getDeltaTime();
                transformComponent->rotation.y += inputComponent->rotationInput.y * rotateComponent->speed * EngineLoopManager::getDeltaTime();
                transformComponent->rotation.z += inputComponent->rotationInput.z * rotateComponent->speed * EngineLoopManager::getDeltaTime();

                if (transformComponent->rotation.x > 360.0f) {
                    transformComponent->rotation.x = 0.0f;
                }
            }
        }

        virtual const std::vector<ComponentType>& GetComponentTypes() const override {
            static std::vector<ComponentType> types = {
                typeid(EngineTransformComponent),
                typeid(PlayerRotateComponent),
                typeid(PlayerInputComponent)
            };
            return types;
        }
    };

    //REGISTER_SYSTEM(PlayerRotateSystem);
}
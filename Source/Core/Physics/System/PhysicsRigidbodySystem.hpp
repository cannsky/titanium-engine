#pragma once

#include "EngineSystem.hpp"
//#include "PhysicsRigidbodyComponent.hpp"
#include "EngineTransformComponent.hpp"
#include "EngineLoopManager.hpp"
#include "EngineSystemRegistration.hpp"
#include "EngineDebugManager.hpp"

namespace TulparEngine::Physics {
    class PhysicsRigidbodySystem : public EngineSystem {
    public:
        PhysicsRigidbodySystem();

        virtual void UpdateBatch(EngineArchetype& archetype, size_t startIdx, size_t endIdx) override;

        virtual const std::vector<ComponentType>& GetComponentTypes() const override {
            static std::vector<ComponentType> types = {
                typeid(EngineTransformComponent)
            };
            return types;
        }
    };
}
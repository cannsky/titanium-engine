#pragma once

#include "EngineEntity.hpp"
#include "EngineComponent.hpp"
#include "EngineComponentRegistration.hpp"

namespace TulparEngine::Examples::SimpleShooter {
    // PlayerTargetComponent derived from EngineComponent
    class PlayerTargetComponent : public EngineComponent {
    public:
        virtual ComponentType GetType() const override {
            return typeid(PlayerTargetComponent);
        }

        EntityID targetEntityID = 0;
        float targetX = 0.0f;
        float targetZ = 0.0f;
        float targetRotX = 0.0f;
        float targetHealth = 100.0f;
    };

    // Register PlayerTargetComponent
    //REGISTER_COMPONENT(PlayerTargetComponent, "PlayerTargetComponent");
}
#pragma once

#include "EngineComponent.hpp"
#include "EngineComponentRegistration.hpp"

namespace TulparEngine::Examples::SimpleShooter {
    // PlayerMoveComponent derived from EngineComponent
    class PlayerMoveComponent : public EngineComponent {
    public:
        virtual ComponentType GetType() const override {
            return typeid(PlayerMoveComponent);
        }

        float speed = 2.0f;
    };

    // Register PlayerMoveComponent
    //REGISTER_COMPONENT(PlayerMoveComponent, "PlayerMoveComponent");
}
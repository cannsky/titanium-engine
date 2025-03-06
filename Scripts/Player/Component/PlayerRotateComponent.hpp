#pragma once

#include "EngineComponent.hpp"
#include "EngineComponentRegistration.hpp"

namespace TulparEngine::Examples::SimpleShooter {
    // PlayerRotateComponent derived from EngineComponent
    class PlayerRotateComponent : public EngineComponent {
    public:
        virtual ComponentType GetType() const override {
            return typeid(PlayerRotateComponent);
        }

        float speed = 10.0f;
    };

    // Register PlayerRotateComponent
    //REGISTER_COMPONENT(PlayerRotateComponent, "PlayerRotateComponent");
}
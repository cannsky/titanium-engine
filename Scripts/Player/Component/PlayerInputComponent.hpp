#pragma once

#include "EngineComponent.hpp"
#include "EngineComponentRegistration.hpp"

#include <glm/glm.hpp>

namespace TulparEngine::Examples::SimpleShooter {
    // PlayerInputComponent derived from EngineComponent
    class PlayerInputComponent : public EngineComponent {
    public:
        virtual ComponentType GetType() const override {
            return typeid(PlayerInputComponent);
        }

        // glm vec3 for input
        glm::vec3 movementInput = glm::vec3(0.0f);
        glm::vec3 rotationInput = glm::vec3(0.0f);
        bool isAttacking = true;
    };

    // Register PlayerInputComponent
    //REGISTER_COMPONENT(PlayerInputComponent, "PlayerInputComponent");
}
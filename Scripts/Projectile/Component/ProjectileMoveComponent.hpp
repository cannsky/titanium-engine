#pragma once

#include "EngineComponent.hpp"
#include "EngineComponentRegistration.hpp"

#include <glm/glm.hpp>

namespace TulparEngine::Examples::SimpleShooter {
    // ProjectileMoveComponent derived from EngineComponent
    class ProjectileMoveComponent : public EngineComponent {
    public:
        virtual ComponentType GetType() const override {
            return typeid(ProjectileMoveComponent);
        }

        float speed = 2.0f;
        glm::vec3 direction = glm::vec3(0.0f);
    };
}
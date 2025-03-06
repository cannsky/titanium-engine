#pragma once

#include "EngineComponent.hpp"
#include "EngineComponentRegistration.hpp"

namespace TulparEngine::Examples::SimpleShooter {
    // ProjectileLifeComponent derived from EngineComponent
    class ProjectileLifeComponent : public EngineComponent {
    public:
        virtual ComponentType GetType() const override {
            return typeid(ProjectileLifeComponent);
        }

        float lifeTime = 5.0f;
        float currentLifeTime = 0.0f;
    };
}
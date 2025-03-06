#pragma once

#include "EngineComponent.hpp"
#include "EngineComponentRegistration.hpp"

namespace TulparEngine::Examples::SimpleShooter {
    // ProjectileStatsComponent derived from EngineComponent
    class ProjectileStatsComponent : public EngineComponent {
    public:
        virtual ComponentType GetType() const override {
            return typeid(ProjectileStatsComponent);
        }

        float damage = 10.0f;
        bool isDestroyed = false;
    };
}
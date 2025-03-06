#pragma once

#include "EngineEntity.hpp"
#include "EngineComponent.hpp"
#include "EngineComponentRegistration.hpp"

namespace TulparEngine::Examples::SimpleShooter {
    // ProjectileTargetComponent derived from EngineComponent
    class ProjectileTargetComponent : public EngineComponent {
    public:
        virtual ComponentType GetType() const override {
            return typeid(ProjectileTargetComponent);
        }

        EntityID ownerEntityID;
        EntityID targetEntityID;
    };
}
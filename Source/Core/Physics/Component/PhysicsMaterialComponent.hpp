#pragma once

#include "EngineComponent.hpp"
#include "EngineComponentRegistration.hpp"
#include <memory>

namespace TulparEngine::Physics {
    class PhysicsMaterialComponent : public EngineComponent {
    public:
        PhysicsMaterialComponent();
        virtual ComponentType GetType() const override;

        float friction;
        float restitution;
        float density;
    };
}
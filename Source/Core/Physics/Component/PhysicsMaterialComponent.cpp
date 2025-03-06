#include "PhysicsMaterialComponent.hpp"

namespace TulparEngine::Physics {
    PhysicsMaterialComponent::PhysicsMaterialComponent()
        : friction(0.5f),
          restitution(0.5f),
          density(1.0f)
    { }

    ComponentType PhysicsMaterialComponent::GetType() const {
        return typeid(PhysicsMaterialComponent);
    }

    REGISTER_COMPONENT(PhysicsMaterialComponent, "Physics Material")
}
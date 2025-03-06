#include "PhysicsColliderComponent.hpp"

namespace TulparEngine::Physics {
    PhysicsColliderComponent::PhysicsColliderComponent()
        : colliderType(ColliderType::Box),
          halfExtents(glm::vec3(0.5f)),
          radius(0.5f),
          capsuleRadius(0.5f),
          capsuleHeight(1.0f),
          collisionLayer(0xFFFFFFFF),
          collisionMask(0xFFFFFFFF),
          isTrigger(false) // Initialize isTrigger
    { }

    ComponentType PhysicsColliderComponent::GetType() const {
        return typeid(PhysicsColliderComponent);
    }

    REGISTER_COMPONENT(PhysicsColliderComponent, "Collider")
}
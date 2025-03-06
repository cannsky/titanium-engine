#include "PhysicsRigidbodyComponent.hpp"

namespace TulparEngine::Physics {
    PhysicsRigidbodyComponent::PhysicsRigidbodyComponent(std::shared_ptr<btRigidBody> rigidBody) 
            : rigidBody(rigidBody) {}

    ComponentType PhysicsRigidbodyComponent::GetType() const {
        return typeid(PhysicsRigidbodyComponent);
    }

    REGISTER_COMPONENT(PhysicsRigidbodyComponent, "Rigidbody")
}
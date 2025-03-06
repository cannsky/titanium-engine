#pragma once
#include "EngineComponent.hpp"
#include "EngineComponentRegistration.hpp"
#include <memory>
#include <btBulletDynamicsCommon.h>

namespace TulparEngine::Physics {
    class PhysicsRigidbodyComponent : public EngineComponent {
    public:
        PhysicsRigidbodyComponent() : rigidBody(nullptr) {}
        PhysicsRigidbodyComponent(std::shared_ptr<btRigidBody> rigidBody);

        virtual ComponentType GetType() const override;

        std::shared_ptr<btRigidBody> rigidBody;

        // Keep these shared pointers alive
        std::shared_ptr<btCollisionShape> collisionShape;
        std::shared_ptr<btMotionState> motionState;
    };
}
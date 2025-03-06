#include "PhysicsRigidbodySystem.hpp"

namespace TulparEngine::Physics {
    PhysicsRigidbodySystem::PhysicsRigidbodySystem() { }

    void PhysicsRigidbodySystem::UpdateBatch(EngineArchetype& archetype, size_t startIdx, size_t endIdx) {
        /*
        // For each entity in this archetype batch
        for (size_t i = startIdx; i < endIdx; ++i) {
            // Get required components
            auto transformComp = std::static_pointer_cast<EngineTransformComponent>(
                archetype.GetComponent(i, typeid(EngineTransformComponent))
            );
            if (rigidbodyComp && rigidbodyComp->rigidBody) {
                // Get the Bullet transform
                btTransform btTrans;
                rigidbodyComp->rigidBody->getMotionState()->getWorldTransform(btTrans);
                // Extract position and rotation from btTransform
                btVector3 btPos = btTrans.getOrigin();
                btQuaternion btRot = btTrans.getRotation();
                // Convert to glm
                glm::vec3 position(btPos.x(), btPos.y(), btPos.z());
                glm::quat rotation(btRot.w(), btRot.x(), btRot.y(), btRot.z());
                // Update the EngineTransformComponent
                transformComp->translation = position;
                // Convert quaternion to euler if your engine uses Euler angles:
                glm::vec3 eulerAngles = glm::eulerAngles(rotation);
                transformComp->rotation = eulerAngles;
            }
        }
        */
    }
}
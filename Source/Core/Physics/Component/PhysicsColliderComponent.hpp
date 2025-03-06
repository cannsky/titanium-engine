#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "EngineComponent.hpp"
#include "EngineComponentRegistration.hpp"

using namespace TulparEngine;

namespace TulparEngine::Physics {
    enum class ColliderType {
        Box,
        Sphere,
        Capsule
    };

    class PhysicsColliderComponent : public EngineComponent {
    public:
        PhysicsColliderComponent();
        virtual ComponentType GetType() const override;
        ColliderType colliderType;
        glm::vec3 halfExtents;
        float radius;
        float capsuleRadius;
        float capsuleHeight;
        uint32_t collisionLayer;
        uint32_t collisionMask;
        bool isTrigger; // Added isTrigger flag
    };
}
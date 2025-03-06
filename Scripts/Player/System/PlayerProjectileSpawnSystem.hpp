#pragma once

#include "EngineSystem.hpp"
#include "EngineSystemRegistration.hpp"
#include "EngineEntityManager.hpp"
#include "EngineDebugManager.hpp"
#include "EngineLoopManager.hpp"
#include "PlayerAttackComponent.hpp"
#include "PlayerStatsComponent.hpp"
#include "PlayerTargetComponent.hpp"

// Projectile Components

#include "ProjectileMoveComponent.hpp"
#include "ProjectileLifeComponent.hpp"
#include "ProjectileStatsComponent.hpp"
#include "ProjectileTargetComponent.hpp"

namespace TulparEngine::Examples::SimpleShooter {
    // PlayerMoveSystem derived from EngineSystem
    class PlayerProjectileSpawnSystem : public EngineSystem {
    public:
        virtual void UpdateBatch(EngineArchetype& archetype, size_t startIdx, size_t endIdx) override {
            for (size_t i = startIdx; i < endIdx; ++i) {

                auto transformComponent = std::static_pointer_cast<EngineTransformComponent>(
                    archetype.GetComponent(i, typeid(EngineTransformComponent)));

                auto attackComponent = std::static_pointer_cast<PlayerAttackComponent>(
                    archetype.GetComponent(i, typeid(PlayerAttackComponent)));

                auto statsComponent = std::static_pointer_cast<PlayerStatsComponent>(
                    archetype.GetComponent(i, typeid(PlayerStatsComponent)));

                auto targetComponent = std::static_pointer_cast<PlayerTargetComponent>(
                    archetype.GetComponent(i, typeid(PlayerTargetComponent)));

                if (attackComponent->isAttacked) {
                    // Reset the flag
                    attackComponent->isAttacked = false;

                    // Calculate look direction
                    glm::vec3 lookDirection = CalculateLookDirection(*transformComponent);

                    // Create a projectile entity
                    std::vector<std::shared_ptr<EngineComponent>> projectileComponents;

                    // Add the ProjectileMoveComponent with the look direction
                    auto projectileMoveComponent = std::make_shared<ProjectileMoveComponent>();
                    auto projectileLifeComponent = std::make_shared<ProjectileLifeComponent>();
                    auto projectileStatsComponent = std::make_shared<ProjectileStatsComponent>();
                    auto projectileTargetComponent = std::make_shared<ProjectileTargetComponent>();
                    
                    projectileMoveComponent->direction = lookDirection;
                    projectileComponents.push_back(projectileMoveComponent);

                    projectileLifeComponent->lifeTime = 5.0f; // Can be changed from here (5 secs is good for now)
                    projectileComponents.push_back(projectileLifeComponent);

                    // Add Projectile Stats Component and Set damage for the projectile
                    projectileStatsComponent->damage = statsComponent->damage;
                    projectileComponents.push_back(projectileStatsComponent);

                    // Add Projectile Target Component and Set target for the projectile
                    projectileTargetComponent->targetEntityID = targetComponent->targetEntityID;
                    EntityID ownerEntityID = archetype.entities[i];
                    projectileTargetComponent->ownerEntityID = ownerEntityID;
                    projectileComponents.push_back(projectileTargetComponent);

                    glm::vec3 projectileStartPosition = glm::vec3(transformComponent->translation.x, transformComponent->translation.y - 1.0f, transformComponent->translation.z);

                    // Create the entity using CreateStaticEntity
                    std::string modelPath = "Resources/Models/Polygon Dungeons/FBX/SM_Env_Door_01.fbx";
                    std::string texturePath = "Resources/Models/Polygon Dungeons/Textures/Dungeons_Texture_01_A.png";
                    EngineEntityManager::GetInstance().CreateStaticEntity(
                        projectileStartPosition, // Start at player position
                        glm::vec3(0.0f),                 // No initial rotation
                        glm::vec3(0.2f),                 // Scale for the projectile
                        "Projectile",                    // Name of the entity (Displayed on the editor)
                        modelPath,
                        texturePath,
                        projectileComponents            // Additional components for the projectile
                    );
                }
            }
        }

        glm::vec3 CalculateLookDirection(const EngineTransformComponent& transform)
        {
            // The full world matrix (including translation & scaling)
            glm::mat4 matrix = transform.mat4();

            // We only want rotation, ignoring translation, so use w=0
            glm::vec4 localForward(-1.0f, 0.0f, 0.0f, 0.0f);
            glm::vec3 worldForward = glm::vec3(matrix * localForward);

            // Normalize
            return glm::normalize(worldForward);
        }

        virtual const std::vector<ComponentType>& GetComponentTypes() const override {
            static std::vector<ComponentType> types = {
                typeid(EngineTransformComponent),
                typeid(PlayerAttackComponent),
                typeid(PlayerStatsComponent),
                typeid(PlayerTargetComponent)
            };
            return types;
        }
    };

    //REGISTER_SYSTEM(PlayerProjectileSpawnSystem);
}

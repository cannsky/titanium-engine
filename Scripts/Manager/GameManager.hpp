#pragma once

#include "EngineEntity.hpp"

#include <absl/container/flat_hash_map.h>
#include <string>
#include <memory>

#include "EngineComponentRegistration.hpp"
#include "EngineSystemRegistration.hpp"

#include "EngineRLAgentComponent.hpp"

#include "PlayerInputComponent.hpp"
#include "PlayerMoveComponent.hpp"
#include "PlayerRotateComponent.hpp"
#include "PlayerAttackComponent.hpp"
#include "PlayerStatsComponent.hpp"
#include "PlayerTargetComponent.hpp"
#include "PlayerRewardComponent.hpp"

#include "PlayerMoveSystem.hpp"
#include "PlayerRotateSystem.hpp"
#include "PlayerAttackSystem.hpp"
#include "PlayerStatsSystem.hpp"
#include "PlayerTargetSystem.hpp"
#include "PlayerProjectileSpawnSystem.hpp"

#include "ProjectileMoveComponent.hpp"
#include "ProjectileLifeComponent.hpp"
#include "ProjectileStatsComponent.hpp"
#include "ProjectileTargetComponent.hpp"

#include "ProjectileMoveSystem.hpp"
#include "ProjectileLifeSystem.hpp"
#include "ProjectileHitSystem.hpp"

#include "EngineEntityManager.hpp"

namespace TulparEngine::Examples::SimpleShooter {
    // PlayerEntity derived from EngineEntity
    class GameManager {
    public:
        // Singleton access method for the Engine
        static GameManager& GetInstance() {
            static GameManager instance;
            return instance;
        }

        void Init() {
            CreatePlayers();
            CreateStaticEntity("Physics Cube", "Resources/Models/Polygon Dungeons/FBX/SM_Env_Door_01.fbx", "Resources/Models/Polygon Dungeons/Textures/Dungeons_Texture_01_A.png");
        }

        void CreatePlayers()
        {
            player1Entity = CreateEntity(
                "Player", 
                "Resources/Custom/Animations/Idle.fbx", 
                "Resources/Models/Polygon Dungeons/Textures/Dungeons_Texture_01_A.png",
                {3.0f, 0.0f, 10.0f}, 
                {0.0f, 0.0f, 0.0f}
            );
            player2Entity = CreateEntity(
                "Player2", 
                "Resources/Custom/Animations/Idle.fbx", 
                "Resources/Models/Polygon Dungeons/Textures/Dungeons_Texture_01_A.png",
                {-1.0f, 0.0f, 10.0f}, 
                {3.0f, 0.0f, 0.0f}
            );
            std::shared_ptr<PlayerTargetComponent> targetComponent = std::make_shared<PlayerTargetComponent>();
            targetComponent->targetEntityID = player2Entity;
            EngineEntityManager::GetInstance().AddComponentToEntity(player1Entity, targetComponent);

            std::shared_ptr<EngineRLAgentComponent> rlAgentComponent = std::make_shared<EngineRLAgentComponent>();
            rlAgentComponent->isPlayerOne = true;
            EngineEntityManager::GetInstance().AddComponentToEntity(player1Entity, rlAgentComponent);

            targetComponent = std::make_shared<PlayerTargetComponent>();
            targetComponent->targetEntityID = player1Entity;
            EngineEntityManager::GetInstance().AddComponentToEntity(player2Entity, targetComponent);

            rlAgentComponent = std::make_shared<EngineRLAgentComponent>();
            rlAgentComponent->isPlayerOne = false;
            EngineEntityManager::GetInstance().AddComponentToEntity(player2Entity, rlAgentComponent);
        }

        void Reset()
        {
            EngineEntityManager::GetInstance().RemoveEntity(player1Entity);
            EngineEntityManager::GetInstance().RemoveEntity(player2Entity);
            CreatePlayers();
        }

        void Start();

        void Run() 
        {
            // check if players are dead and reset the game
            auto player1Stats = EngineEntityManager::GetInstance().GetComponent<PlayerStatsComponent>(player1Entity);
            auto player2Stats = EngineEntityManager::GetInstance().GetComponent<PlayerStatsComponent>(player2Entity);
            if (player1Stats->isDead || player2Stats->isDead)
            {
                EngineDebugManager::GetInstance().log("Game Over", DebugCategory::UserDebug);
                EngineDebugManager::GetInstance().log("-----------------------------------------", DebugCategory::UserDebug);
                Reset();
            }
        }

        void Cleanup();

        EntityID CreateEntity(const std::string entityName, const std::string& modelPath, const std::string& texturePath, const glm::vec3& position, const glm::vec3& rotation) {
            std::vector<std::shared_ptr<EngineComponent>> modelComponents;

            // Add Player components
            std::shared_ptr<PlayerAttackComponent> attackComponent = std::make_shared<PlayerAttackComponent>();
            std::shared_ptr<PlayerInputComponent> inputComponent = std::make_shared<PlayerInputComponent>();
            std::shared_ptr<PlayerMoveComponent> moveComponent = std::make_shared<PlayerMoveComponent>();
            std::shared_ptr<PlayerRotateComponent> rotateComponent = std::make_shared<PlayerRotateComponent>();
            std::shared_ptr<PlayerStatsComponent> statsComponent = std::make_shared<PlayerStatsComponent>();
            std::shared_ptr<PlayerRewardComponent> rewardComponent = std::make_shared<PlayerRewardComponent>();

            modelComponents.push_back(attackComponent);
            modelComponents.push_back(inputComponent);
            modelComponents.push_back(moveComponent);
            modelComponents.push_back(rotateComponent);
            modelComponents.push_back(statsComponent);
            modelComponents.push_back(rewardComponent);

            EntityID entityID = EngineEntityManager::GetInstance().CreateEntity(position, rotation, glm::vec3(0.01f, -0.01f, 0.01f), entityName, modelPath, texturePath, modelComponents);
            return entityID;
        }

        EntityID CreateStaticEntity(const std::string entityName, const std::string& modelPath, const std::string& texturePath) {
            EntityID entityID = EngineEntityManager::GetInstance().CreateStaticEntity({0.4f, 1.0f, 10.0f}, {0.0f, 0.0f, 0.0f}, glm::vec3(1.0f, 1.0f, 1.0f), "Physics Cube", modelPath, texturePath);
            return entityID;
        }

        EntityID player1Entity;
        EntityID player2Entity;

        // Deleting copy constructor and assignment operator to enforce singleton
        GameManager(const GameManager&) = delete;
        void operator=(const GameManager&) = delete;

    private:
        GameManager() {}
        ~GameManager() {}
    };

    REGISTER_COMPONENT(PlayerInputComponent, "PlayerInputComponent");
    REGISTER_COMPONENT(PlayerMoveComponent, "PlayerMoveComponent");
    REGISTER_COMPONENT(PlayerRotateComponent, "PlayerRotateComponent");
    REGISTER_COMPONENT(PlayerAttackComponent, "PlayerAttackComponent");
    REGISTER_COMPONENT(PlayerStatsComponent, "PlayerStatsComponent");
    REGISTER_COMPONENT(PlayerTargetComponent, "PlayerTargetComponent");
    REGISTER_COMPONENT(PlayerRewardComponent, "PlayerRewardComponent");

    REGISTER_COMPONENT(ProjectileMoveComponent, "ProjectileMoveComponent");
    REGISTER_COMPONENT(ProjectileLifeComponent, "ProjectileLifeComponent");
    REGISTER_COMPONENT(ProjectileStatsComponent, "ProjectileStatsComponent");
    REGISTER_COMPONENT(ProjectileTargetComponent, "ProjectileTargetComponent");

    REGISTER_SYSTEM(PlayerMoveSystem);
    REGISTER_SYSTEM(PlayerRotateSystem);
    REGISTER_SYSTEM(PlayerAttackSystem);
    REGISTER_SYSTEM(PlayerProjectileSpawnSystem);
    REGISTER_SYSTEM(PlayerStatsSystem);
    REGISTER_SYSTEM(PlayerTargetSystem);

    REGISTER_SYSTEM(ProjectileMoveSystem);
    REGISTER_SYSTEM(ProjectileLifeSystem);
    REGISTER_SYSTEM(ProjetileHitSystem);
}
#include "Test.hpp"
#include <iostream>

#include "EngineComponentRegistration.hpp"

#include "PlayerInputComponent.hpp"
#include "PlayerMoveComponent.hpp"
#include "PlayerRotateComponent.hpp"
#include "PlayerAttackComponent.hpp"
#include "PlayerStatsComponent.hpp"

#include "PlayerMoveSystem.hpp"
#include "PlayerRotateSystem.hpp"
#include "PlayerAttackSystem.hpp"
#include "PlayerProjectileSpawnSystem.hpp"

#include "ProjectileMoveComponent.hpp"
#include "ProjectileLifeComponent.hpp"
#include "ProjectileStatsComponent.hpp"
#include "ProjectileMoveSystem.hpp"

using namespace TulparEngine::Examples::SimpleShooter;

namespace TulparEngine {
    Test::Test() { } // Default constructor definition
    Test::~Test() { } // Destructor definition

    void Test::Init() {
        // Implement initialization logic here
        addSystemsAndComponents();
        //testAnimations();
        CreatePlayerEntity("Player", "Resources/Custom/Animations/Idle.fbx", "Resources/Models/Polygon Dungeons/Textures/Dungeons_Texture_01_A.png");
        //testAnimations();
        testPhysics();
    }

    // Define the run function
    void Test::run() {
        // Implement the main loop here
    }

    // Define the cleanup function
    void Test::cleanup() {
        // Implement cleanup logic here
    }

    void Test::testAnimations() {
        std::string modelPath1 = "Resources/Custom/Animations/Idle.fbx";
        std::string texturePath1 = "Resources/Models/Polygon Dungeons/Textures/Dungeons_Texture_01_A.png";

        std::vector<std::shared_ptr<EngineComponent>> modelComponents;

        // Add Player components
        std::shared_ptr<PlayerAttackComponent> attackComponent = std::make_shared<PlayerAttackComponent>();
        std::shared_ptr<PlayerInputComponent> inputComponent = std::make_shared<PlayerInputComponent>();
        std::shared_ptr<PlayerMoveComponent> moveComponent = std::make_shared<PlayerMoveComponent>();
        std::shared_ptr<PlayerRotateComponent> rotateComponent = std::make_shared<PlayerRotateComponent>();
        std::shared_ptr<PlayerStatsComponent> statsComponent = std::make_shared<PlayerStatsComponent>();

        modelComponents.push_back(attackComponent);
        modelComponents.push_back(inputComponent);
        modelComponents.push_back(moveComponent);
        modelComponents.push_back(rotateComponent);
        modelComponents.push_back(statsComponent);

        EngineEntityManager::GetInstance().CreateEntity({2.0f, 0.0f, 10.0f}, {0.0f, 0.0f, 0.0f}, glm::vec3(0.01f, 0.01f, 0.01f), "Dangerous Goblin", modelPath1, texturePath1, modelComponents);
    }

    void Test::CreatePlayerEntity(const std::string entityName, const std::string& modelPath, const std::string& texturePath) {
        std::vector<std::shared_ptr<EngineComponent>> modelComponents;

        // Add Player components
        std::shared_ptr<PlayerAttackComponent> attackComponent = std::make_shared<PlayerAttackComponent>();
        std::shared_ptr<PlayerInputComponent> inputComponent = std::make_shared<PlayerInputComponent>();
        std::shared_ptr<PlayerMoveComponent> moveComponent = std::make_shared<PlayerMoveComponent>();
        std::shared_ptr<PlayerRotateComponent> rotateComponent = std::make_shared<PlayerRotateComponent>();
        std::shared_ptr<PlayerStatsComponent> statsComponent = std::make_shared<PlayerStatsComponent>();

        modelComponents.push_back(attackComponent);
        modelComponents.push_back(inputComponent);
        modelComponents.push_back(moveComponent);
        modelComponents.push_back(rotateComponent);
        modelComponents.push_back(statsComponent);

        EngineEntityManager::GetInstance().CreateEntity({2.0f, 0.0f, 10.0f}, {0.0f, 0.0f, 0.0f}, glm::vec3(0.01f, 0.01f, 0.01f), entityName, modelPath, texturePath, modelComponents);
    }

    void Test::testPhysics() {
        std::string modelPath1 = "Resources/Models/Polygon Pirates/FBX/SM_Prop_Barrel_01.fbx";
        std::string texturePath1 = "Resources/Models/Polygon Pirates/Textures/Texture_01_A.png";

        EngineEntityManager::GetInstance().CreateStaticEntity({0.4f, 1.0f, 10.0f}, {0.0f, 0.0f, 0.0f}, glm::vec3(1.0f, 1.0f, 1.0f), "Physics Cube", modelPath1, texturePath1);
    }

    void Test::addSystemsAndComponents() {
        // Player

        // Register systems
        EngineEntityManager::GetInstance().AddSystem(std::make_unique<PlayerMoveSystem>());
        EngineEntityManager::GetInstance().AddSystem(std::make_unique<PlayerRotateSystem>());
        EngineEntityManager::GetInstance().AddSystem(std::make_unique<PlayerAttackSystem>());
        EngineEntityManager::GetInstance().AddSystem(std::make_unique<PlayerProjectileSpawnSystem>());

        // Projectile

        // Register systems
        EngineEntityManager::GetInstance().AddSystem(std::make_unique<ProjectileMoveSystem>());
    }

    REGISTER_COMPONENT(PlayerInputComponent, "PlayerInputComponent");
    REGISTER_COMPONENT(PlayerMoveComponent, "PlayerMoveComponent");
    REGISTER_COMPONENT(PlayerRotateComponent, "PlayerRotateComponent");
    REGISTER_COMPONENT(PlayerAttackComponent, "PlayerAttackComponent");
    REGISTER_COMPONENT(PlayerStatsComponent, "PlayerStatsComponent");

    REGISTER_COMPONENT(ProjectileMoveComponent, "ProjectileMoveComponent");
    REGISTER_COMPONENT(ProjectileLifeComponent, "ProjectileLifeComponent");
    REGISTER_COMPONENT(ProjectileStatsComponent, "ProjectileStatsComponent");
}
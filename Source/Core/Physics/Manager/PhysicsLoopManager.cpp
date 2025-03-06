#include "PhysicsLoopManager.hpp"

namespace TulparEngine::Physics {

    PhysicsLoopManager::PhysicsLoopManager()
        : fixedDeltaTime(1.0f / 60.0f), accumulator(0.0f), currentTime(0.0f) { }

    PhysicsLoopManager::~PhysicsLoopManager() { }

    void PhysicsLoopManager::Init() {
        // Initialization if needed
    }

    void PhysicsLoopManager::Start() {
        // Start code if needed
    }

    void PhysicsLoopManager::Run() {
        // This method is likely called every frame
        // Accumulate deltaTime and run FixedUpdate as needed.
        float deltaTime = TulparEngine::EngineLoopManager::GetInstance().getDeltaTime();
        currentTime += deltaTime;
        accumulator += deltaTime;

        while (accumulator >= fixedDeltaTime) {
            FixedUpdate();
            accumulator -= fixedDeltaTime;
        }
    }

    void PhysicsLoopManager::FixedUpdate() {
        // Step the Bullet physics world
        int maxSubSteps = 1;
        float fixedTimeStep = fixedDeltaTime;

        // If you have a global dynamicsWorld, step it:
        // dynamicsWorld->stepSimulation(fixedDeltaTime, maxSubSteps, fixedTimeStep);

        // After stepping the simulation, update physics systems
        PhysicsSystemManager::GetInstance().Update();
    }

    void PhysicsLoopManager::Cleanup() {
        // Cleanup if needed
    }

}
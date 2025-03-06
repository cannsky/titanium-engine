/*#include "PhysicsEngine.hpp"

namespace TulparEngine::Physics {

    PhysicsEngine::PhysicsEngine() {}

    PhysicsEngine::~PhysicsEngine() {}

    void PhysicsEngine::Init() {
        PhysicsLoopManager::GetInstance().Init();
        PhysicsSystemManager::GetInstance().AddSystem(std::make_unique<PhysicsRigidbodySystem>());

        // Example (simplified):
        broadphase = new btDbvtBroadphase();
        collisionConfiguration = new btDefaultCollisionConfiguration();
        dispatcher = new btCollisionDispatcher(collisionConfiguration);
        solver = new btSequentialImpulseConstraintSolver();
        dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

        dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));
    }

    void PhysicsEngine::Start() {

    }

    void PhysicsEngine::Run() {
        PhysicsLoopManager::GetInstance().Run();
    }

    void PhysicsEngine::Cleanup() {
        
    }

}*/
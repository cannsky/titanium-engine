/*
#pragma once

#include <btBulletDynamicsCommon.h>
#include "PhysicsSystemManager.hpp"
#include "PhysicsLoopManager.hpp"
#include "PhysicsRigidbodySystem.hpp"

namespace TulparEngine::Physics {
    class PhysicsEngine {
    public:
        // Singleton access method for the Engine
        static PhysicsEngine& GetInstance() {
            static PhysicsEngine instance;
            return instance;
        }

        void Init();
        void Start();
        void Run();
        void Cleanup();

        // Deleting copy constructor and assignment operator to enforce singleton
        PhysicsEngine(const PhysicsEngine&) = delete;
        void operator=(const PhysicsEngine&) = delete;

        // Boolean for debug mode
        bool isDebugModeEnabled = true;

        // Boolean for editor mode
        bool isEditorEnabled = true;

        // Boolean for test mode
        bool isTestModeEnabled = true;

        btDynamicsWorld* GetDynamicsWorld() { return dynamicsWorld; }
    private:
        PhysicsEngine();  // Private constructor for singleton
        ~PhysicsEngine();

        btDynamicsWorld* dynamicsWorld = nullptr;
        btBroadphaseInterface* broadphase = nullptr;
        btCollisionDispatcher* dispatcher = nullptr;
        btConstraintSolver* solver = nullptr;
        btCollisionConfiguration* collisionConfiguration = nullptr;
    };
}*/
#pragma once

#include <string>
#include <iostream>
#include <cassert>
#include "EngineDebugManager.hpp"

namespace TulparEngine {
    class EnginePhysicsManager {
    public:
        // Singleton access method for EngineFrameManager
        static EnginePhysicsManager& GetInstance() {
            static EnginePhysicsManager instance;
            return instance;
        }

        void Init();
        void Start();
        void Run();
        void Cleanup();

        // Deleting copy constructor and assignment operator to enforce singleton
        EnginePhysicsManager(const EnginePhysicsManager&) = delete;
        void operator=(const EnginePhysicsManager&) = delete;
    private:
        EnginePhysicsManager();  // Private constructor for singleton
        ~EnginePhysicsManager();
    };
}
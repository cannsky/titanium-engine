#pragma once

#include <vector>
#include <memory>

#include "EngineSystem.hpp"

#include "EngineLoopManager.hpp"
#include "PhysicsSystemManager.hpp"
#include "EngineEntityManager.hpp"

namespace TulparEngine::Physics {
    class PhysicsLoopManager {
    public:
        static PhysicsLoopManager& GetInstance() {
            static PhysicsLoopManager instance;
            return instance;
        }

        void Init();
        void Start();
        void Run();
        void Cleanup();

    private:
        PhysicsLoopManager();
        ~PhysicsLoopManager();

        float fixedDeltaTime;
        float accumulator;
        float currentTime;

        void FixedUpdate();
    };
}
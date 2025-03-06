#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <mutex>
#include <thread>
#include "EngineSystem.hpp"
#include "EngineArchetype.hpp"
#include "EngineEntityManager.hpp"
#include "EngineDebugManager.hpp"

namespace TulparEngine::Physics {

    class PhysicsSystemManager {
    public:
        static PhysicsSystemManager& GetInstance() {
            static PhysicsSystemManager instance;
            return instance;
        }

        // Add a physics system
        void AddSystem(std::unique_ptr<EngineSystem> system) {
            physicsSystems.push_back(std::move(system));
        }

        // Update all physics systems
        void Update();

        // Prevent copy
        PhysicsSystemManager(const PhysicsSystemManager&) = delete;
        void operator=(const PhysicsSystemManager&) = delete;

    private:
        PhysicsSystemManager() = default;
        ~PhysicsSystemManager() = default;

        std::vector<std::unique_ptr<EngineSystem>> physicsSystems;

        // Helper method to run system on matching archetypes
        void RunSystemOnArchetype(EngineSystem& system, EngineArchetype& archetype);
    };
}
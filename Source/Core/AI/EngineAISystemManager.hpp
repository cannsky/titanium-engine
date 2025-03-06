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

namespace TulparEngine {

    class EngineAISystemManager {
    public:
        static EngineAISystemManager& GetInstance() {
            static EngineAISystemManager instance;
            return instance;
        }

        // Add a ai system
        void AddSystem(std::unique_ptr<EngineSystem> system) {
            aiSystems.push_back(std::move(system));
        }

        // Update all ai systems
        void Update();

        // Prevent copy
        EngineAISystemManager(const EngineAISystemManager&) = delete;
        void operator=(const EngineAISystemManager&) = delete;

    private:
        EngineAISystemManager() = default;
        ~EngineAISystemManager() = default;

        std::vector<std::unique_ptr<EngineSystem>> aiSystems;

        // Helper method to run system on matching archetypes
        void RunSystemOnArchetype(EngineSystem& system, EngineArchetype& archetype);
    };
}
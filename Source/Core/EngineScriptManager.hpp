#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <cassert>

#include "EngineScript.hpp"

namespace TulparEngine {
    class EngineScriptManager {
    public:
        // Singleton access method for EngineScriptManager
        static EngineScriptManager& GetInstance() {
            static EngineScriptManager instance;
            return instance;
        }

        // Initialize system
        void Init();
        // Run system
        void run(float deltaTime);
        // Cleanup system
        void cleanup();

        // Initialize all scripts
        void initScripts();
        // Run all scripts
        void runScripts(float deltaTime);
        // Cleanup all scripts
        void cleanupScripts();

        // Add a script to the manager
        void addScript(std::shared_ptr<EngineScript> script);
        // Remove a script from the manager
        void removeScript(std::shared_ptr<EngineScript> script);

        // Deleting copy constructor and assignment operator to enforce singleton
        EngineScriptManager(const EngineScriptManager&) = delete;
        void operator=(const EngineScriptManager&) = delete;
    private:
        EngineScriptManager();  // Private constructor for singleton
        ~EngineScriptManager();

        // Store all scripts
        std::vector<std::shared_ptr<EngineScript>> scripts;
    };
}
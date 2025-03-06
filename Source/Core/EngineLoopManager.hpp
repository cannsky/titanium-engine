#pragma once

#include <string>
#include <iostream>
#include <cassert>
#include <chrono>

#include "Editor.hpp"
#include "EngineAIManager.hpp"
#include "EngineFrameManager.hpp"
#include "EngineEntityManager.hpp"

namespace TulparEngine {
    class EngineLoopManager {
    public:
        // Singleton access method for EngineLoopManager
        static EngineLoopManager& GetInstance() {
            static EngineLoopManager instance;
            return instance;
        }

        // Initialize all systems
        void Init();

        // Run the main engine loop
        void run();

        // Cleanup all systems
        void cleanup();

        // Get the current frame
        static int frame;

        // Get the delta time
        static float getDeltaTime() { return GetInstance().deltaTime; }

        // Deleting copy constructor and assignment operator to enforce singleton
        EngineLoopManager(const EngineLoopManager&) = delete;
        void operator=(const EngineLoopManager&) = delete;
    private:
        EngineLoopManager();  // Private constructor for singleton
        ~EngineLoopManager();

        // Calculate delta time
        void calculateDeltaTime();

        // Time variables
        std::chrono::time_point<std::chrono::high_resolution_clock> newTime;
        std::chrono::time_point<std::chrono::high_resolution_clock> lastTime;
        float deltaTime;
    };
}
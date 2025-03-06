#pragma once

#include <string>
#include <iostream>
#include <cassert>
#include "EngineDebugManager.hpp"
#include "EngineVulkanManager.hpp"
#include "EngineGraphicsManager.hpp"
#include "EngineWindowManager.hpp"

namespace TulparEngine {
    class EngineFrameManager {
    public:
        // Singleton access method for EngineFrameManager
        static EngineFrameManager& GetInstance() {
            static EngineFrameManager instance;
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

        // Deleting copy constructor and assignment operator to enforce singleton
        EngineFrameManager(const EngineFrameManager&) = delete;
        void operator=(const EngineFrameManager&) = delete;
    private:
        EngineFrameManager();  // Private constructor for singleton
        ~EngineFrameManager();

        void newFrame();
    };
}
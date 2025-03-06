#pragma once

#include "EngineAISystemManager.hpp"

#include "EngineDebugManager.hpp"

namespace TulparEngine {
    class EngineAIManager {
    public:
        // Singleton access method for EngineAIManager
        static EngineAIManager& GetInstance() {
            static EngineAIManager instance;
            return instance;
        }

        void Init();
        void Start();
        void Run();
        void Cleanup();

        // Get the current frame
        static int frame;

        // Deleting copy constructor and assignment operator to enforce singleton
        EngineAIManager(const EngineAIManager&) = delete;
        void operator=(const EngineAIManager&) = delete;
    private:
        EngineAIManager();  // Private constructor for singleton
        ~EngineAIManager();
    };
}
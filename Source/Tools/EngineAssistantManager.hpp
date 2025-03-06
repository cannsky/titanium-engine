#pragma once

#include <cstdlib>
#include <iostream>
#include <csignal>
#include <unistd.h>

#ifndef ENGINE_DIR
#define ENGINE_DIR "../"
#endif

#include "EngineDebugManager.hpp"

namespace TulparEngine::Tools {
    class EngineAssistantManager {
    public:
        // Singleton access method for EngineAssistantManager
        static EngineAssistantManager& GetInstance() {
            static EngineAssistantManager instance;
            return instance;
        }

        void Init();
        void Start();
        void Run();
        void Cleanup();

        // Deleting copy constructor and assignment operator to enforce singleton
        EngineAssistantManager(const EngineAssistantManager&) = delete;
        void operator=(const EngineAssistantManager&) = delete;
    private:
        EngineAssistantManager();  // Private constructor for singleton
        ~EngineAssistantManager();

        pid_t npmProcessPid = -1;

        void StartAssistantServer();
        void StopAssistantServer();
    };
}
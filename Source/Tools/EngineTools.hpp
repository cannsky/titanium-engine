#pragma once

#include "EngineAssistantManager.hpp"
#include "EngineBrowserManager.hpp"
#include "EngineRequestManager.hpp"

namespace TulparEngine::Tools {
    class EngineTools {
    public:
        // Singleton access method for the EngineTools
        static EngineTools& GetInstance() {
            static EngineTools instance;
            return instance;
        }

        void Init();
        void Start();
        void Run();
        void Cleanup();

        // Deleting copy constructor and assignment operator to enforce singleton
        EngineTools(const EngineTools&) = delete;
        void operator=(const EngineTools&) = delete;

        // Boolean for debug mode
        bool isAssistantEnabled = true;
        bool isBrowserEnabled = true;
        bool isRequestEnabled = true;
    private:
        EngineTools();  // Private constructor for singleton
        ~EngineTools();
    };
}
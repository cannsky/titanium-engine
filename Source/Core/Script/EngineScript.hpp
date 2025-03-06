// EngineScript.hpp
#pragma once

#include <memory>
#include "EngineScriptManager.hpp"

namespace TulparEngine {
    class EngineScriptManager;
    
    class EngineScript : public std::enable_shared_from_this<EngineScript> {
    public:
        // Add the script to the script manager when it is created
        EngineScript();

        // Remove the script from the script manager when it is destroyed
        virtual ~EngineScript();

        // Initialize the script
        virtual void Init() = 0;

        // Run the script
        virtual void run(float deltaTime) = 0;

        // Cleanup the script
        virtual void cleanup() = 0;
    };
}
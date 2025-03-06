#pragma once

#include <string>

#include "EngineComponent.hpp"
#include "EngineComponentRegistration.hpp"

namespace TulparEngine {
    class EngineAIAgentComponent : public EngineComponent {
    public:
        EngineAIAgentComponent() {

        }

        virtual ComponentType GetType() const override {
            return typeid(EngineAIAgentComponent);
        }
        
        std::string name;
        std::string context;
        std::string input = "1 + 1 = ?";
        std::string response;

        bool isProcessing;
        bool isDone;
    };

    REGISTER_COMPONENT(EngineAIAgentComponent, "AI Agent")
}
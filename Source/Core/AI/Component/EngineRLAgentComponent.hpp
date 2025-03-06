#pragma once

#include <string>
#include "EngineComponent.hpp"
#include "EngineComponentRegistration.hpp"

namespace TulparEngine {
    class EngineRLAgentComponent : public EngineComponent {
    public:
        EngineRLAgentComponent() {}

        virtual ComponentType GetType() const override {
            return typeid(EngineRLAgentComponent);
        }

        bool isTraining = true;
        bool isActionRecieved = false;
        int chosenAction = -1;

        // New: Distinguish Player1 or Player2
        bool isPlayerOne = false; 
        // If 'isPlayerOne = false', we treat it as Player2.
    };

    REGISTER_COMPONENT(EngineRLAgentComponent, "RL Agent")
}
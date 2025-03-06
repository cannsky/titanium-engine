#pragma once

#include <string>
#include <vector>
#include <functional>
#include <absl/container/flat_hash_map.h>

#include "EngineComponent.hpp"
#include "EngineState.hpp"
#include "EngineComponentRegistration.hpp"

namespace TulparEngine {
    class EngineStateMachineComponent : public EngineComponent {
    public:
        EngineStateMachineComponent() {

        }

        virtual ComponentType GetType() const override {
            return typeid(EngineStateMachineComponent);
        }

        EngineState currentState;
        absl::flat_hash_map<StateID, EngineState> states;
    };

    REGISTER_COMPONENT(EngineStateMachineComponent, "State Machine")
}
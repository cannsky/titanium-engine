#pragma once

#include "EngineEntity.hpp"
#include "EngineStateMachineComponent.hpp"

#include "EngineEntityManager.hpp"
#include "EngineDebugManager.hpp"

namespace TulparEngine {
    class EngineStateMachineFactory {
    public:
        // Singleton access method for EngineFrameManager
        static EngineStateMachineFactory& GetInstance() {
            static EngineStateMachineFactory instance;
            return instance;
        }

        void Init();
        void Start();
        void Run();
        void Cleanup();

        static EntityID CreateStateMachine();
        static void AddState(EntityID entityID, StateID stateID, const EngineState& state);
        static void RemoveState(EntityID entityID, StateID stateID);
        static void AddTransition(EntityID entityID, StateID fromStateID, StateID toStateID, const std::function<bool()>& condition);
        static void RemoveTransition(EntityID entityID, StateID fromStateID, StateID toStateID);
        static void SetInitialState(EntityID entityID, StateID stateID);

        // Deleting copy constructor and assignment operator to enforce singleton
        EngineStateMachineFactory(const EngineStateMachineFactory&) = delete;
        void operator=(const EngineStateMachineFactory&) = delete;
    private:
        EngineStateMachineFactory();  // Private constructor for singleton
        ~EngineStateMachineFactory();
    };
}
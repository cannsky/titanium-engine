#include "EngineStateMachineFactory.hpp"

namespace TulparEngine {
    EngineStateMachineFactory::EngineStateMachineFactory() {}
    EngineStateMachineFactory::~EngineStateMachineFactory() {}
    void EngineStateMachineFactory::Init() {}
    void EngineStateMachineFactory::Start() {}
    void EngineStateMachineFactory::Run() {}
    void EngineStateMachineFactory::Cleanup() {}

    EntityID EngineStateMachineFactory::CreateStateMachine() {
        // Create state machine component
        std::shared_ptr<EngineStateMachineComponent> stateMachineComponent = std::make_shared<EngineStateMachineComponent>();
        // Create components array
        std::vector<std::shared_ptr<EngineComponent>> components;
        // Add state machine component to components array
        components.push_back(stateMachineComponent);
        // Create a new entity
        EntityID entityID = EngineEntityManager::GetInstance().CreateEntity(components);
    }

    void EngineStateMachineFactory::AddState(EntityID entityID, StateID stateID, const EngineState& state) {
        // Get the state machine component of the entity
        // EngineEntityManager::GetInstance().GetComponent<EngineStateMachineComponent>(entityID);
        std::shared_ptr<EngineStateMachineComponent> stateMachineComponent = 
            std::static_pointer_cast<EngineStateMachineComponent>(EngineEntityManager::GetInstance().GetComponent(entityID, typeid(EngineStateMachineComponent)));
        // Add the state to the state machine component
        stateMachineComponent->states[stateID] = state;
    }

    void EngineStateMachineFactory::RemoveState(EntityID entityID, StateID stateID) {
        // Get the state machine component of the entity
        std::shared_ptr<EngineStateMachineComponent> stateMachineComponent = 
            std::static_pointer_cast<EngineStateMachineComponent>(EngineEntityManager::GetInstance().GetComponent(entityID, typeid(EngineStateMachineComponent)));
        // Remove the state from the state machine component
        stateMachineComponent->states.erase(stateID);
    }

    void EngineStateMachineFactory::AddTransition(EntityID entityID, StateID fromStateID, StateID toStateID, const std::function<bool()>& condition) {
        // Get the state machine component of the entity
        std::shared_ptr<EngineStateMachineComponent> stateMachineComponent =
            std::static_pointer_cast<EngineStateMachineComponent>(EngineEntityManager::GetInstance().GetComponent(entityID, typeid(EngineStateMachineComponent)));
        // Add the transition to the state machine component
        stateMachineComponent->states[fromStateID].transitions[toStateID].targetStateID = toStateID;
        stateMachineComponent->states[fromStateID].transitions[toStateID].condition = condition;
    }

    void EngineStateMachineFactory::RemoveTransition(EntityID entityID, StateID fromStateID, StateID toStateID) {
        // Get the state machine component of the entity
        std::shared_ptr<EngineStateMachineComponent> stateMachineComponent =
            std::static_pointer_cast<EngineStateMachineComponent>(EngineEntityManager::GetInstance().GetComponent(entityID, typeid(EngineStateMachineComponent)));
        // Remove the transition from the state machine component
        stateMachineComponent->states[fromStateID].transitions.erase(toStateID);
    }

    void EngineStateMachineFactory::SetInitialState(EntityID entityID, StateID stateID) {
        // Get the state machine component of the entity
        std::shared_ptr<EngineStateMachineComponent> stateMachineComponent =
            std::static_pointer_cast<EngineStateMachineComponent>(EngineEntityManager::GetInstance().GetComponent(entityID, typeid(EngineStateMachineComponent)));
        // Set the initial state of the state machine component
        stateMachineComponent->currentState = stateMachineComponent->states[stateID];
    }
}
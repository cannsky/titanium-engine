#include "EngineModelComponent.hpp"
#include "EngineComponentRegistration.hpp"

namespace TulparEngine {

    EngineModelComponent::EngineModelComponent() {
        // Constructor code
    }

    ComponentType EngineModelComponent::GetType() const {
        return typeid(EngineModelComponent);
    }

    REGISTER_COMPONENT(EngineModelComponent, "Model")
}
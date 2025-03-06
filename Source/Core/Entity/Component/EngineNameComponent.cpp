#include "EngineNameComponent.hpp"
#include "EngineComponentRegistration.hpp"

namespace TulparEngine {

    EngineNameComponent::EngineNameComponent() {
        // Constructor code
    }

    ComponentType EngineNameComponent::GetType() const {
        return typeid(EngineNameComponent);
    }

    REGISTER_COMPONENT(EngineNameComponent, "Name")
}
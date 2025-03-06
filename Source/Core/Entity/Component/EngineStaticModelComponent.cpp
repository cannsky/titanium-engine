#include "EngineStaticModelComponent.hpp"
#include "EngineComponentRegistration.hpp"

namespace TulparEngine {

    EngineStaticModelComponent::EngineStaticModelComponent() {
        // Constructor code
    }

    ComponentType EngineStaticModelComponent::GetType() const {
        return typeid(EngineStaticModelComponent);
    }

    REGISTER_COMPONENT(EngineStaticModelComponent, "StaticModel")
}
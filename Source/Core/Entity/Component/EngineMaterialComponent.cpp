#include "EngineMaterialComponent.hpp"
#include "EngineComponentRegistration.hpp"

namespace TulparEngine {
    
    EngineMaterialComponent::EngineMaterialComponent() {
        // Constructor code
    }

    ComponentType EngineMaterialComponent::GetType() const {
        return typeid(EngineMaterialComponent);
    }

    REGISTER_COMPONENT(EngineMaterialComponent, "Material")
}
#pragma once

#include <glm/glm.hpp>
#include "EngineComponent.hpp"

namespace TulparEngine {
    class EngineMaterialComponent : public EngineComponent {
    public:
        EngineMaterialComponent();
        virtual ComponentType GetType() const override;
        
        glm::vec3 color{};
    };
}
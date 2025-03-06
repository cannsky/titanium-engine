#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "EngineStaticModel.hpp"
#include "EngineComponent.hpp"

namespace TulparEngine {
    class EngineStaticModelComponent : public EngineComponent {
    public:
        EngineStaticModelComponent();
        virtual ComponentType GetType() const override;
        
        std::shared_ptr<EngineStaticModel> model{};
        glm::vec3 color{};
    };
}
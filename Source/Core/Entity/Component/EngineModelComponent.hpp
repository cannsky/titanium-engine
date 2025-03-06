#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "EngineModel.hpp"
#include "EngineComponent.hpp"

namespace TulparEngine {
    class EngineModelComponent : public EngineComponent {
    public:
        EngineModelComponent();
        virtual ComponentType GetType() const override;
        
        std::shared_ptr<EngineModel> model{};
        glm::vec3 color{};
    };
}
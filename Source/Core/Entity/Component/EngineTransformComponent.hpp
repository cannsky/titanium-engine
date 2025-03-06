#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "EngineComponent.hpp"

namespace TulparEngine {
    class EngineTransformComponent : public EngineComponent {
    public:
        EngineTransformComponent();
        virtual ComponentType GetType() const override;
        glm::vec3 translation{};
        glm::vec3 scale{1.0f, 1.0f, 1.0f};
        glm::vec3 rotation;
        
        glm::mat4 mat4() const;
        glm::mat3 normalMatrix();
    };
}
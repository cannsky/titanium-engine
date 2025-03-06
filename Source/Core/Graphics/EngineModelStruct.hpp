#pragma once

#include <glm/glm.hpp>

namespace TulparEngine {
    struct SimplePushConstantData {
        glm::mat4 modelMatrix{1.f};
        glm::mat4 normalMatrix{1.f};
    };
}
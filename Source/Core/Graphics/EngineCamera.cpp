#include "EngineCamera.hpp"

#include <cassert>
#include <limits>

namespace TulparEngine {
    void EngineCamera::setOrthographicProjection(float left, float right, float top, float bottom, float near, float far) {
        projectionMatrix = glm::mat4(1.0f);
        projectionMatrix[0][0] = 2.0f / (right - left);
        projectionMatrix[1][1] = 2.0f / (bottom - top);
        projectionMatrix[2][2] = 1.0f / (far - near);
        projectionMatrix[3][0] = -(right + left) / (right - left);
        projectionMatrix[3][1] = -(bottom + top) / (bottom - top);
        projectionMatrix[3][2] = -near / (far - near);
    }

    void EngineCamera::setPerspectiveProjection(float fov, float aspect, float near, float far) {
        assert(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
        const float tanHalfFov = glm::tan(fov / 2.0f);
        projectionMatrix = glm::mat4(0.0f);
        projectionMatrix[0][0] = 1.0f / (aspect * tanHalfFov);
        projectionMatrix[1][1] = -1.0f / tanHalfFov; // Invert Y-axis for Vulkan
        projectionMatrix[2][2] = far / (far - near);
        projectionMatrix[2][3] = 1.0f;
        projectionMatrix[3][2] = -(near * far) / (far - near);
    }

    void EngineCamera::setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up) {
        const glm::vec3 w{glm::normalize(direction)};
        const glm::vec3 u{glm::normalize(glm::cross(w, up))};
        const glm::vec3 v{glm::cross(w, u)};

        viewMatrix = glm::mat4{1.f};
        viewMatrix[0][0] = u.x;
        viewMatrix[1][0] = u.y;
        viewMatrix[2][0] = u.z;
        viewMatrix[0][1] = v.x;
        viewMatrix[1][1] = v.y;
        viewMatrix[2][1] = v.z;
        viewMatrix[0][2] = w.x;
        viewMatrix[1][2] = w.y;
        viewMatrix[2][2] = w.z;
        viewMatrix[3][0] = -glm::dot(u, position);
        viewMatrix[3][1] = -glm::dot(v, position);
        viewMatrix[3][2] = -glm::dot(w, position);
    }

    void EngineCamera::setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up) {
        setViewDirection(position, target - position, up);
    }

    void EngineCamera::setViewYXZ(glm::vec3 position, glm::vec3 rotation) {
        // Calculate direction vector from Euler angles
        glm::vec3 direction{
            glm::sin(rotation.y) * glm::cos(rotation.x),
            glm::sin(rotation.x),
            glm::cos(rotation.y) * glm::cos(rotation.x)
        };

        // In Vulkan, the camera looks along the negative Z-axis
        // So we invert the direction vector
        direction = -direction;

        // Define the up vector (Y-axis pointing down in Vulkan)
        glm::vec3 up{0.f, -1.f, 0.f};

        // Use glm::lookAt to create the view matrix
        viewMatrix = glm::lookAt(position, position + direction, up);
    }
}
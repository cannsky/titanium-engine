#include "EngineInputManager.hpp"

namespace TulparEngine {
    
    int EngineInputManager::frame = 0;

    EngineInputManager::EngineInputManager() {}

    EngineInputManager::~EngineInputManager() { cleanup(); }

    void EngineInputManager::Init() {

    }

    void EngineInputManager::run() {

    }

    void EngineInputManager::cleanup() {

    }

    void EngineInputManager::moveInPlaneXZ(float deltaTime, EngineGameObject& gameObject) {
        if (EngineEditor::EditorAssistantChatUI::GetInstance().GetIsMouseOverImage()) {
            return;
        }
        glm::vec3 rotate{0};
        if (glfwGetKey(EngineWindowManager::getWindowPointer(), keys.lookRight) == GLFW_PRESS) rotate.y += 1.f;
        if (glfwGetKey(EngineWindowManager::getWindowPointer(), keys.lookLeft) == GLFW_PRESS) rotate.y -= 1.f;
        if (glfwGetKey(EngineWindowManager::getWindowPointer(), keys.lookUp) == GLFW_PRESS) rotate.x += 1.f;
        if (glfwGetKey(EngineWindowManager::getWindowPointer(), keys.lookDown) == GLFW_PRESS) rotate.x -= 1.f;

        if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
            gameObject.transform.rotation += lookSpeed * deltaTime * glm::normalize(rotate);
        }

        // limit pitch values between about +/- 85ish degrees
        gameObject.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
        gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());

        float yaw = gameObject.transform.rotation.y;
        const glm::vec3 forwardDir{sin(yaw), 0.f, cos(yaw)};
        const glm::vec3 rightDir{forwardDir.z, 0.f, -forwardDir.x};
        const glm::vec3 upDir{0.f, -1.f, 0.f};

        glm::vec3 moveDir{0.f};
        if (glfwGetKey(EngineWindowManager::getWindowPointer(), keys.moveForward) == GLFW_PRESS) moveDir += forwardDir;
        if (glfwGetKey(EngineWindowManager::getWindowPointer(), keys.moveBackward) == GLFW_PRESS) moveDir -= forwardDir;
        if (glfwGetKey(EngineWindowManager::getWindowPointer(), keys.moveRight) == GLFW_PRESS) moveDir += rightDir;
        if (glfwGetKey(EngineWindowManager::getWindowPointer(), keys.moveLeft) == GLFW_PRESS) moveDir -= rightDir;
        if (glfwGetKey(EngineWindowManager::getWindowPointer(), keys.moveUp) == GLFW_PRESS) moveDir += upDir;
        if (glfwGetKey(EngineWindowManager::getWindowPointer(), keys.moveDown) == GLFW_PRESS) moveDir -= upDir;

        if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
            gameObject.transform.translation += moveSpeed * deltaTime * glm::normalize(moveDir);
        }
    }
}
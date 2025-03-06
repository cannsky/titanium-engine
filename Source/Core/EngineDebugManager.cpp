#include "EngineDebugManager.hpp"

namespace TulparEngine {
    EngineDebugManager::EngineDebugManager() {}

    EngineDebugManager::~EngineDebugManager() {}

    void EngineDebugManager::Init() {}

    void EngineDebugManager::run() {}

    void EngineDebugManager::log(const std::string& message, DebugCategory category) {
        switch (category) {
            case DebugCategory::GraphicsManager:
                if (GetInstance().isGraphicsPipelineDebug) {
                    std::cout << "[Graphics Manager] " << message << std::endl;
                }
                break;
            case DebugCategory::WindowManager:
                if (GetInstance().isWindowManagerDebug) {
                    std::cout << "[Window Manager] " << message << std::endl;
                }
                break;
            case DebugCategory::GraphicsPipeline:
                if (GetInstance().isGraphicsPipelineDebug) {
                    std::cout << "[Graphics Pipeline] " << message << std::endl;
                }
                break;
            case DebugCategory::Renderer:
                if (GetInstance().isRendererDebug) {
                    std::cout << "[Renderer] " << message << std::endl;
                }
                break;
            case DebugCategory::VulkanManager:
                if (GetInstance().isVulkanManagerDebug) {
                    std::cout << "[Vulkan Manager] " << message << std::endl;
                }
                break;
            case DebugCategory::FrameManager:
                if (GetInstance().isFrameManagerDebug) {
                    std::cout << "[Frame Manager] " << message << std::endl;
                }
                break;
            case DebugCategory::Error:
                if (GetInstance().isErrorDebug) {
                    std::cerr << "[Error] " << message << std::endl;
                }
                break;
            case DebugCategory::Model:
                if (GetInstance().isModelDebug) {
                    std::cout << "[Model] " << message << std::endl;
                }
                break;
            case DebugCategory::UserDebug:
                if (GetInstance().isUserDebug) {
                    std::cout << "[Debug] " << message << std::endl;
                }
                break;
            case DebugCategory::EntityManager:
                if (GetInstance().isEntityManagerDebug) {
                    std::cout << "[Entity Manager] " << message << std::endl;
                }
                break;
            case DebugCategory::SceneManager:
                if (GetInstance().isSceneManagerDebug) {
                    std::cout << "[Scene Manager] " << message << std::endl;
                }
                break;
            case DebugCategory::Scene:
                if (GetInstance().isSceneDebug) {
                    std::cout << "[Scene] " << message << std::endl;
                }
                break;
            case DebugCategory::PhysicsCollisions:
                if (GetInstance().isPhysicsCollisionsDebug) {
                    std::cout << "[Physics Collisions] " << message << std::endl;
                }
                break;
            case DebugCategory::Animation:
                if (GetInstance().isAnimationDebug) {
                    std::cout << "[Animation] " << message << std::endl;
                }
                break;
            case DebugCategory::Assistant:
                if (GetInstance().isAssistantDebug) {
                    std::cout << "[Assistant] " << message << std::endl;
                }
                break;
            case DebugCategory::BrowserManager:
                if (GetInstance().isBrowserManagerDebug) {
                    std::cout << "[Browser Manager] " << message << std::endl;
                }
                break;
            case DebugCategory::Physics:
                if (GetInstance().isPhysicsDebug) {
                    std::cout << "[Physics] " << message << std::endl;
                }
                break;
            case DebugCategory::AI:
                if (GetInstance().isAIDebug) {
                    std::cout << "[AI] " << message << std::endl;
                }
                break;
        }
    }

    void EngineDebugManager::logEngineError(const std::string& message) {
        throw std::runtime_error(message);
    }

    void EngineDebugManager::logMatrix(const std::string& message, const glm::mat4& matrix) {
        std::cout << message << std::endl;
        for (int i = 0; i < 4; i++) {
            std::cout << matrix[i][0] << " " << matrix[i][1] << " " << matrix[i][2] << " " << matrix[i][3] << std::endl;
        }
    }

    void EngineDebugManager::cleanup() {}
}
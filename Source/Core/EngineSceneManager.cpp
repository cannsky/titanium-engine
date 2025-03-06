#include "EngineSceneManager.hpp"

namespace TulparEngine {

    EngineSceneManager::EngineSceneManager() {}

    EngineSceneManager::~EngineSceneManager() {}

    void EngineSceneManager::Init() {
        createScene("Default");
        setActiveScene("Default");
    }

    void EngineSceneManager::start() {
        if (activeScene) {
            activeScene->start();
        }
    }

    void EngineSceneManager::run() {
        if (activeScene) {
            activeScene->run();
        }
    }

    void EngineSceneManager::cleanup() {
        if (activeScene) {
            activeScene->cleanup();
        }
    }

    void EngineSceneManager::createScene(const std::string& sceneName) {
        if (scenes.find(sceneName) == scenes.end()) {
            scenes[sceneName] = std::make_shared<EngineScene>();
        }
    }

    void EngineSceneManager::removeScene(const std::string& sceneName) {
        if (scenes.find(sceneName) != scenes.end()) {
            if (activeScene == scenes[sceneName]) {
                activeScene->cleanup();
                activeScene = nullptr;
            }
            scenes.erase(sceneName);
        }
    }

    std::shared_ptr<EngineScene> EngineSceneManager::getScene(const std::string& sceneName) {
        if (scenes.find(sceneName) != scenes.end()) {
            return scenes[sceneName];
        }
        return nullptr; // Return null if scene not found
    }

    void EngineSceneManager::setActiveScene(const std::string& sceneName) {
        auto scene = getScene(sceneName);
        if (scene) {
            if (activeScene) {
                activeScene->cleanup();
            }
            activeScene = scene;
            activeScene->Init();
        }
    }

    std::shared_ptr<EngineScene> EngineSceneManager::getCurrentScene() const {
        return activeScene;
    }
}
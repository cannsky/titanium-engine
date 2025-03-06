#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <vector>

#include "EngineScene.hpp"

namespace TulparEngine {
    class EngineSceneManager {
    public:
        static EngineSceneManager& GetInstance() {
            static EngineSceneManager instance;
            return instance;
        }

        void Init(); // Initialize the scene manager
        void start(); // Start the active scene
        void run(); // Run the active scene (called every frame)
        void cleanup(); // Cleanup the active scene

        void createScene(const std::string& sceneName);
        void removeScene(const std::string& sceneName);
        std::shared_ptr<EngineScene> getScene(const std::string& sceneName);
        void setActiveScene(const std::string& sceneName);
        std::shared_ptr<EngineScene> getCurrentScene() const;

        // Deleting copy constructor and assignment operator to enforce singleton
        EngineSceneManager(const EngineSceneManager&) = delete;
        void operator=(const EngineSceneManager&) = delete;

    private:
        EngineSceneManager(); // Private constructor for singleton
        ~EngineSceneManager(); // Destructor implementation

        std::unordered_map<std::string, std::shared_ptr<EngineScene>> scenes;
        std::shared_ptr<EngineScene> activeScene = nullptr;
    };
}

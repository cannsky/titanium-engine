#pragma once

#include <memory>

#include "EngineEntityManager.hpp"
#include "PlayerEntity.hpp"
#include "EngineSceneManager.hpp"

namespace TulparEngine {
    class Test {
    public:
        // Singleton access method for the Engine
        static Test& GetInstance() {
            static Test instance;
            return instance;
        }

        void Init();
        void start();
        void run();
        void cleanup();

        void addSystemsAndComponents();
        void testAnimations();
        void testPhysics();
        void createEntity();
        void CreatePlayerEntity(const std::string entityName, const std::string& modelPath, const std::string& texturePath);

        // Deleting copy constructor and assignment operator to enforce singleton
        Test(const Test&) = delete;
        void operator=(const Test&) = delete;
    private:
        Test();  // Private constructor for singleton
        ~Test();
    };
}
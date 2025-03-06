#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include "EngineEntity.hpp"
#include <memory>
#include <string>

namespace TulparEngine {

    struct TransformComponent {
        glm::vec3 translation{};
        glm::vec3 scale{1.0f, 1.0f, 1.0f};
        glm::vec3 rotation;
        
        glm::mat4 mat4();
        glm::mat3 normalMatrix();
    };

    class EngineGameObject {
    public:
        using uint = unsigned int;
        
        std::string name;

        // Orijinal statik fonksiyon: doğrudan nesne döndürür
        static EngineGameObject createGameObject() {
            static uint currentId = 0;
            return EngineGameObject(currentId++);
        }

        // Yeni statik fonksiyon: unique_ptr döndürür
        static std::unique_ptr<EngineGameObject> createUniqueGameObject() {
            static uint currentId = 0;
            return std::make_unique<EngineGameObject>(currentId++);
        }

        void SetEntityID(EntityID id) {
            entityID = id;
        }
        
        EngineGameObject(const EngineGameObject& ) = delete;
        EngineGameObject& operator=(const EngineGameObject&) = delete;
        EngineGameObject(EngineGameObject&& ) = default;
        EngineGameObject& operator=(EngineGameObject&&) = default;

        uint getId() const { return id; }

        glm::vec3 color{};
        TransformComponent transform{};

        EntityID entityID;

        EngineGameObject(uint id) : id(id), entityID(0) {} // Initialize entityID
    private:
        uint id;
    };
}
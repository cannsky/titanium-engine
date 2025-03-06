#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
#include "EngineEntity.hpp"
#include "EngineDebugManager.hpp"

namespace TulparEngine {

    struct SceneEntityData {
        EntityID entityID;
        std::string name;
    };

    class EngineScene {
    public:
        EngineScene();
        ~EngineScene();

        void Init();
        void start();
        void run();
        void cleanup();

        void AddEntity(EntityID entityID, const std::string& name);
        void RemoveEntity(EntityID entityID);
        const std::vector<SceneEntityData>& GetOrderedEntities() const;

    private:
        std::vector<SceneEntityData> orderedEntities;
        std::unordered_map<EntityID, size_t> entityIndexMap; // To quickly find and remove entities
    };
}
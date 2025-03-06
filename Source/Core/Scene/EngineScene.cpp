#include "EngineScene.hpp"

namespace TulparEngine {

    EngineScene::EngineScene() {}
    EngineScene::~EngineScene() {}

    void EngineScene::Init() {}
    void EngineScene::start() {}
    void EngineScene::run() {}
    void EngineScene::cleanup() {}

    void EngineScene::AddEntity(EntityID entityID, const std::string& name) {
        if (entityIndexMap.find(entityID) == entityIndexMap.end()) {
            orderedEntities.push_back({entityID, name});
            entityIndexMap[entityID] = orderedEntities.size() - 1;

            EngineDebugManager::log("Added entity " + std::to_string(entityID) + " to scene with name " + name, DebugCategory::Scene);
        }
    }

    void EngineScene::RemoveEntity(EntityID entityID) {
        auto it = entityIndexMap.find(entityID);
        if (it != entityIndexMap.end()) {
            size_t index = it->second;
            orderedEntities.erase(orderedEntities.begin() + index);
            entityIndexMap.erase(it);

            // Update the index map to reflect new positions
            for (size_t i = index; i < orderedEntities.size(); ++i) {
                entityIndexMap[orderedEntities[i].entityID] = i;
            }

            EngineDebugManager::log("Removed entity " + std::to_string(entityID) + " from scene", DebugCategory::Scene);
        }
    }

    const std::vector<SceneEntityData>& EngineScene::GetOrderedEntities() const {
        return orderedEntities;
    }
}
#pragma once

#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <string>
#include <vector>
#include <memory>
#include <typeindex>
#include <mutex>
#include <thread>
#include <queue>
#include <bitset>

#include "EngineSceneManager.hpp"

#include "EngineEntity.hpp"
#include "EngineStaticModel.hpp"
#include "EngineArchetype.hpp"
#include "EngineSystem.hpp"
#include "EngineTypes.hpp"

#include "EngineTransformComponent.hpp"
#include "EngineModelComponent.hpp"
#include "EngineStaticModelComponent.hpp"
#include "EngineNameComponent.hpp"
#include "EngineAnimatorComponent.hpp"

namespace TulparEngine {

    using ComponentType = std::type_index;
    
    enum class EntityOperationType {
        AddEntity,
        RemoveEntity,
        AddComponentToEntity,
        RemoveComponentFromEntity
    };

    struct EntityOperation {
        EntityOperationType type;
        EntityID entityID;
        std::vector<std::shared_ptr<EngineComponent>> components = {};  // Default to empty vector
        ComponentType componentType = ComponentType(typeid(void));      // Default to a placeholder type
    };

    class EngineEntityManager {
    public:
        static EngineEntityManager& GetInstance() {
            static EngineEntityManager instance;
            return instance;
        }

        void Init();
        void start();
        void run();
        void cleanup();

        static EntityID nextEntityID;

        // List of systems
        std::vector<std::unique_ptr<EngineSystem>> systems;

        // List of components
        std::vector<std::unique_ptr<EngineComponent>> components;

        // Add a system to the manager
        void AddSystem(std::unique_ptr<EngineSystem> system) {
            EngineDebugManager::log("Adding system to manager", DebugCategory::EntityManager);
            systems.push_back(std::move(system));
        }

        template <typename... Components, typename Func>
        void each(Func func) {
            for (auto& [signature, archetypePtr] : archetypes) {
                EngineArchetype& archetype = *archetypePtr;
                if (ArchetypeMatchesSystem(archetype, {typeid(Components)...})) {
                    for (EntityID entityID : archetype.entities) {
                        size_t index = archetype.GetEntityIndex(entityID);
                        func(entityID,
                            *std::static_pointer_cast<Components>(archetype.GetComponent(index, typeid(Components)))...);
                    }
                }
            }
        }

        EntityID CreateEntity(const std::vector<std::shared_ptr<EngineComponent>>& components);
        EntityID CreateEntity(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const std::string name, const std::string& modelPath, const std::string& textureBaseDir);
        EntityID CreateEntity(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const std::string name, const std::string& modelPath, const std::string& textureBaseDir, const std::vector<std::shared_ptr<EngineComponent>>& additionalComponents);
        EntityID CreateStaticEntity(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const std::string name, const std::string& modelPath, const std::string& textureBaseDir);
        EntityID CreateStaticEntity(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const std::string name, const std::string& modelPath, const std::string& textureBaseDir, const std::vector<std::shared_ptr<EngineComponent>>& additionalComponents);
        // Public methods for entity and component management
        void AddEntity(EntityID entityID, const std::vector<std::shared_ptr<EngineComponent>>& components);
        void RemoveEntity(EntityID entityID);
        std::vector<std::shared_ptr<EngineComponent>> GetEntityComponents(EntityID entityID);
        void AddComponentToEntity(EntityID entityID, std::shared_ptr<EngineComponent> component);
        void RemoveComponentFromEntity(EntityID entityID, ComponentType componentType);
        void RegisterComponentType(ComponentType componentType, const std::string& name, std::function<std::shared_ptr<EngineComponent>()> factory);
        std::vector<ComponentType> GetAddableComponentTypes(EntityID entityID);
        void AddComponentToEntity(EntityID entityID, ComponentType componentType);
        std::string GetComponentTypeName(ComponentType componentType);

        // Update method to run systems on archetypes
        void Update();

        std::shared_ptr<EngineComponent> GetComponent(EntityID entityID, ComponentType componentType);
        template<typename ComponentType>
        ComponentType* GetComponent(EntityID entityID) {
            auto it = entityToArchetype.find(entityID);
            if (it != entityToArchetype.end()) {
                EngineArchetype* archetype = it->second;
                size_t index = archetype->GetEntityIndex(entityID);
                return archetype->GetComponent<ComponentType>(index);
            }
            return nullptr;
        }

        const std::unordered_map<ComponentSignature, std::unique_ptr<EngineArchetype>>& GetArchetypes() const {
            return archetypes;
        }

        ComponentSignature GetComponentSignature(const std::vector<ComponentType>& componentTypes);

        EngineEntityManager(const EngineEntityManager&) = delete;
        void operator=(const EngineEntityManager&) = delete;
    private:
        EngineEntityManager();
        ~EngineEntityManager();

        // Deferred operations
        std::queue<EntityOperation> deferredOperations;
        std::mutex operationsMutex;
        std::unordered_set<ComponentType> availableComponentTypes;
        std::unordered_map<ComponentType, std::function<std::shared_ptr<EngineComponent>()>> componentFactories;
        std::unordered_map<ComponentType, std::string> componentTypeNames;
        std::unordered_map<ComponentType, size_t> componentTypeToIndex;
        std::unordered_map<ComponentSignature, std::unique_ptr<EngineArchetype>> archetypes;
        size_t nextComponentTypeIndex = 0;

        // Map from EntityID to Archetype
        std::unordered_map<EntityID, EngineArchetype*> entityToArchetype;

        // Find or create an archetype based on component types
        EngineArchetype* GetOrCreateArchetype(const std::vector<ComponentType>& componentTypes);

        // Check if an archetype matches a system's required components
        bool ArchetypeMatchesSystem(const EngineArchetype& archetype, const std::vector<ComponentType>& systemComponentTypes);

        // Run a system on an archetype using multithreading and batching
        void RunSystemOnArchetype(EngineSystem& system, EngineArchetype& archetype);

        // Process deferred operations
        void ProcessDeferredOperations();

        // Process each type of deferred operation
        void ProcessAddEntity(const EntityOperation& op);
        void ProcessRemoveEntity(const EntityOperation& op);
        void ProcessAddComponentToEntity(const EntityOperation& op);
        void ProcessRemoveComponentFromEntity(const EntityOperation& op);
    };
}
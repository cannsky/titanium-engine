#include "EngineEntityManager.hpp"
#include "EngineLoopManager.hpp"
#include "EngineDestructionManager.hpp"

namespace TulparEngine {
    EntityID EngineEntityManager::nextEntityID = 1;

    EngineEntityManager::EngineEntityManager() {
        
    }

    EngineEntityManager::~EngineEntityManager() {
        // Cleanup code here
    }

    void EngineEntityManager::Init() {
        // Initialization code here
    }

    void EngineEntityManager::start() {
        // Start code here
    }

    void EngineEntityManager::run() {
        Update();
    }

    void EngineEntityManager::cleanup() {
        // Cleanup code here
    }

    EntityID EngineEntityManager::CreateEntity(const std::vector<std::shared_ptr<EngineComponent>>& components) {
        EntityID entityID = nextEntityID++;
        AddEntity(entityID, components);
        return entityID;
    }

    EntityID EngineEntityManager::CreateEntity(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const std::string name, const std::string& modelPath, const std::string& textureBaseDir) {
        std::shared_ptr<EngineModel> cubeModel = std::make_shared<EngineModel>(modelPath, textureBaseDir);

        std::vector<std::shared_ptr<EngineComponent>> components;

        // Create entity components
        std::shared_ptr<EngineTransformComponent> transformComponent = std::make_shared<EngineTransformComponent>();
        transformComponent->translation = position;
        transformComponent->scale = scale;
        transformComponent->rotation = rotation;

        std::shared_ptr<EngineModelComponent> modelComponent = std::make_shared<EngineModelComponent>();
        modelComponent->model = cubeModel;

        std::shared_ptr<EngineNameComponent> nameComponent = std::make_shared<EngineNameComponent>();
        nameComponent->name = name;

        std::shared_ptr<EngineAnimatorComponent> animatorComponent = std::make_shared<EngineAnimatorComponent>();

        cubeModel->loadAnimationsFromFile("Resources/Custom/Animations/Idle.fbx");
        cubeModel->loadAnimationsFromFile("Resources/Custom/Animations/Waving.fbx");

        // Add components to the entity
        components.push_back(transformComponent);
        components.push_back(modelComponent);
        components.push_back(nameComponent);
        components.push_back(animatorComponent);

        EntityID entityID = nextEntityID++;
        AddEntity(entityID, components);
        return entityID;
    }

    EntityID EngineEntityManager::CreateEntity(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const std::string name, const std::string& modelPath, const std::string& textureBaseDir, const std::vector<std::shared_ptr<EngineComponent>>& additonalComponents) {
        std::shared_ptr<EngineModel> cubeModel = std::make_shared<EngineModel>(modelPath, textureBaseDir);

        std::vector<std::shared_ptr<EngineComponent>> components;
        
        // Create entity components
        std::shared_ptr<EngineTransformComponent> transformComponent = std::make_shared<EngineTransformComponent>();
        transformComponent->translation = position;
        transformComponent->scale = scale;
        transformComponent->rotation = rotation;

        std::shared_ptr<EngineModelComponent> modelComponent = std::make_shared<EngineModelComponent>();
        modelComponent->model = cubeModel;

        std::shared_ptr<EngineNameComponent> nameComponent = std::make_shared<EngineNameComponent>();
        nameComponent->name = name;

        std::shared_ptr<EngineAnimatorComponent> animatorComponent = std::make_shared<EngineAnimatorComponent>();

        cubeModel->loadAnimationsFromFile("Resources/Custom/Animations/Idle.fbx");
        cubeModel->loadAnimationsFromFile("Resources/Custom/Animations/Waving.fbx");

        // Add components to the entity
        components.push_back(transformComponent);
        components.push_back(modelComponent);
        components.push_back(nameComponent);
        components.push_back(animatorComponent);

        for (const auto& component : additonalComponents) {
            components.push_back(component);
        }

        EntityID entityID = nextEntityID++;
        AddEntity(entityID, components);
        return entityID;
    }

    EntityID EngineEntityManager::CreateStaticEntity(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const std::string name, const std::string& modelPath, const std::string& textureBaseDir) {
        std::shared_ptr<EngineStaticModel> model = std::make_shared<EngineStaticModel>(modelPath, textureBaseDir);

        std::vector<std::shared_ptr<EngineComponent>> components;

        // Create entity components
        std::shared_ptr<EngineTransformComponent> transformComponent = std::make_shared<EngineTransformComponent>();
        transformComponent->translation = position;
        transformComponent->scale = scale;
        transformComponent->rotation = rotation;

        std::shared_ptr<EngineStaticModelComponent> modelComponent = std::make_shared<EngineStaticModelComponent>();
        modelComponent->model = model;

        std::shared_ptr<EngineNameComponent> nameComponent = std::make_shared<EngineNameComponent>();
        nameComponent->name = name;

        std::shared_ptr<EngineAnimatorComponent> animatorComponent = std::make_shared<EngineAnimatorComponent>();

        // Add components to the entity
        components.push_back(transformComponent);
        components.push_back(modelComponent);
        components.push_back(nameComponent);
        components.push_back(animatorComponent);

        EntityID entityID = nextEntityID++;
        AddEntity(entityID, components);
        return entityID;
    }

    EntityID EngineEntityManager::CreateStaticEntity(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const std::string name, const std::string& modelPath, const std::string& textureBaseDir, const std::vector<std::shared_ptr<EngineComponent>>& additonalComponents) {
        std::shared_ptr<EngineStaticModel> model = std::make_shared<EngineStaticModel>(modelPath, textureBaseDir);

        std::vector<std::shared_ptr<EngineComponent>> components;

        // Create entity components
        std::shared_ptr<EngineTransformComponent> transformComponent = std::make_shared<EngineTransformComponent>();
        transformComponent->translation = position;
        transformComponent->scale = scale;
        transformComponent->rotation = rotation;

        std::shared_ptr<EngineStaticModelComponent> modelComponent = std::make_shared<EngineStaticModelComponent>();
        modelComponent->model = model;

        std::shared_ptr<EngineNameComponent> nameComponent = std::make_shared<EngineNameComponent>();
        nameComponent->name = name;

        std::shared_ptr<EngineAnimatorComponent> animatorComponent = std::make_shared<EngineAnimatorComponent>();

        // Add components to the entity
        components.push_back(transformComponent);
        components.push_back(modelComponent);
        components.push_back(nameComponent);
        components.push_back(animatorComponent);

        // Add the additional components
        for (const auto& component : additonalComponents) {
            components.push_back(component);
        }

        EntityID entityID = nextEntityID++;
        AddEntity(entityID, components);
        return entityID;
    }

    void EngineEntityManager::AddEntity(EntityID entityID, const std::vector<std::shared_ptr<EngineComponent>>& components) {
        std::lock_guard<std::mutex> lock(operationsMutex);
        deferredOperations.push({EntityOperationType::AddEntity, entityID, components});
    }

    void EngineEntityManager::RemoveEntity(EntityID entityID) {
        std::lock_guard<std::mutex> lock(operationsMutex);
        deferredOperations.push({EntityOperationType::RemoveEntity, entityID});
    }

    std::vector<std::shared_ptr<EngineComponent>> EngineEntityManager::GetEntityComponents(EntityID entityID) {
        auto it = entityToArchetype.find(entityID);
        if (it != entityToArchetype.end()) {
            EngineArchetype* archetype = it->second;
            size_t index = archetype->GetEntityIndex(entityID);
            EngineDebugManager::log("Found entity with ID: " + std::to_string(entityID), DebugCategory::EntityManager);
            return archetype->GetEntityComponents(index);
        }
        return {}; // Return empty vector if entity not found
    }

    void EngineEntityManager::RegisterComponentType(ComponentType componentType, const std::string& name, std::function<std::shared_ptr<EngineComponent>()> factory) {
        availableComponentTypes.insert(componentType);
        componentFactories[componentType] = factory;
        componentTypeNames[componentType] = name;

        EngineDebugManager::log("Registered component type: " + name, DebugCategory::EntityManager);

        // Assign an index to the component type
        if (componentTypeToIndex.find(componentType) == componentTypeToIndex.end()) {
            componentTypeToIndex[componentType] = nextComponentTypeIndex++;
            if (nextComponentTypeIndex > MAX_COMPONENT_TYPES) {
                throw std::runtime_error("Exceeded maximum number of component types");
            }
        }
    }

    std::vector<ComponentType> EngineEntityManager::GetAddableComponentTypes(EntityID entityID) {
        std::vector<ComponentType> addableComponents;

        // Get the entity's current component types
        auto entityComponents = GetEntityComponents(entityID);
        std::unordered_set<ComponentType> entityComponentTypes;
        for (const auto& component : entityComponents) {
            entityComponentTypes.insert(component->GetType());
        }

        // For each available component type, if the entity does not have it, add to the list
        for (const auto& componentType : availableComponentTypes) {
            if (entityComponentTypes.find(componentType) == entityComponentTypes.end()) {
                addableComponents.push_back(componentType);
            }
        }

        return addableComponents;
    }

    void EngineEntityManager::AddComponentToEntity(EntityID entityID, ComponentType componentType) {
        // Use the factory function to create a new component instance
        auto it = componentFactories.find(componentType);
        if (it != componentFactories.end()) {
            auto component = it->second();
            // Enqueue the operation to add the component to the entity
            AddComponentToEntity(entityID, component);
        } else {
            // Handle error: factory not found
            EngineDebugManager::log("Factory not found for component type", DebugCategory::EntityManager);
        }
    }

    void EngineEntityManager::AddComponentToEntity(EntityID entityID, std::shared_ptr<EngineComponent> component) {
        std::lock_guard<std::mutex> lock(operationsMutex);
        // Enqueue the operation to add the component to the entity
        deferredOperations.push({EntityOperationType::AddComponentToEntity, entityID, {component}, component->GetType()});
    }

    std::string EngineEntityManager::GetComponentTypeName(ComponentType componentType) {
        auto it = componentTypeNames.find(componentType);
        if (it != componentTypeNames.end()) {
            return it->second;
        } else {
            return "Unknown";
        }
    }

    void EngineEntityManager::RemoveComponentFromEntity(EntityID entityID, ComponentType componentType) {
        std::lock_guard<std::mutex> lock(operationsMutex);
        deferredOperations.push({EntityOperationType::RemoveComponentFromEntity, entityID, {}, componentType});
    }

    void EngineEntityManager::Update() {
        for (auto& [signature, archetype] : archetypes) {
            for (auto& system : systems) {
                ComponentSignature systemSignature = GetComponentSignature(system->GetComponentTypes());
                if ((archetype->signature & systemSignature) == systemSignature) {
                    RunSystemOnArchetype(*system, *archetype);
                }
            }
        }
        ProcessDeferredOperations();
    }

    EngineArchetype* EngineEntityManager::GetOrCreateArchetype(const std::vector<ComponentType>& componentTypes) {
        ComponentSignature signature = GetComponentSignature(componentTypes);

        auto it = archetypes.find(signature);
        if (it != archetypes.end()) {
            return it->second.get();
        } else {
            // Create a new archetype
            auto newArchetype = std::make_unique<EngineArchetype>(componentTypes, signature);
            EngineArchetype* archetypePtr = newArchetype.get();
            archetypes[signature] = std::move(newArchetype);
            return archetypePtr;
        }
    }

    bool EngineEntityManager::ArchetypeMatchesSystem(const EngineArchetype& archetype, const std::vector<ComponentType>& systemComponentTypes) {
        for (const auto& type : systemComponentTypes) {
            if (std::find(archetype.componentTypes.begin(), archetype.componentTypes.end(), type) == archetype.componentTypes.end()) {
                return false;
            }
        }
        return true;
    }

    void EngineEntityManager::RunSystemOnArchetype(EngineSystem& system, EngineArchetype& archetype) {
        // Define batch size
        const size_t batchSize = 2; // Adjust as needed
        size_t numEntities = archetype.entities.size();
        size_t numBatches = (numEntities + batchSize - 1) / batchSize;

        std::vector<std::thread> threads;

        for (size_t batch = 0; batch < numBatches; ++batch) {
            size_t startIdx = batch * batchSize;
            size_t endIdx = std::min(startIdx + batchSize, numEntities);

            // For each batch, create a thread to process the entities
            threads.emplace_back([&system, &archetype, startIdx, endIdx]() {
                system.UpdateBatch(archetype, startIdx, endIdx);
            });
        }

        // Wait for all threads to finish
        for (auto& thread : threads) {
            thread.join();
        }
    }

    void EngineEntityManager::ProcessDeferredOperations() {
        std::lock_guard<std::mutex> lock(operationsMutex);
        while (!deferredOperations.empty()) {
            const auto& op = deferredOperations.front();
            switch (op.type) {
                case EntityOperationType::AddEntity:
                    ProcessAddEntity(op);
                    break;
                case EntityOperationType::RemoveEntity:
                    ProcessRemoveEntity(op);
                    break;
                case EntityOperationType::AddComponentToEntity:
                    ProcessAddComponentToEntity(op);
                    break;
                case EntityOperationType::RemoveComponentFromEntity:
                    ProcessRemoveComponentFromEntity(op);
                    break;
            }
            deferredOperations.pop();
        }
    }

    void EngineEntityManager::ProcessAddEntity(const EntityOperation& op) {
        // Determine component types
        std::vector<ComponentType> componentTypes;
        for (const auto& component : op.components) {
            componentTypes.push_back(component->GetType());
        }

        // Find or create the matching archetype
        EngineArchetype* archetype = GetOrCreateArchetype(componentTypes);

        // Add the entity to the archetype
        archetype->AddEntity(op.entityID, op.components);

        // Map entity to archetype
        entityToArchetype[op.entityID] = archetype;

        EngineDebugManager::log("Added entity with ID: " + std::to_string(op.entityID), DebugCategory::EntityManager);

        auto nameComponent = std::dynamic_pointer_cast<EngineNameComponent>(op.components[2]);
        if (nameComponent) {
            EngineSceneManager::GetInstance().getCurrentScene()->AddEntity(op.entityID, nameComponent->name);
        }
    }

    void EngineEntityManager::ProcessRemoveEntity(const EntityOperation& op) {
        /*auto it = entityToArchetype.find(op.entityID);
        if (it != entityToArchetype.end()) {
            EngineArchetype* archetype = it->second;
            archetype->RemoveEntity(op.entityID);
            entityToArchetype.erase(it);
            EngineSceneManager::GetInstance().getCurrentScene()->RemoveEntity(op.entityID);
        } else {
            // Entity not found; handle error if necessary
        }

        EngineDebugManager::log("Removed entity with ID: " + std::to_string(op.entityID), DebugCategory::EntityManager);*/

        auto it = entityToArchetype.find(op.entityID);
        if (it != entityToArchetype.end()) {
            EngineArchetype* archetype = it->second;
            size_t index = archetype->GetEntityIndex(op.entityID);

            // 1. Retrieve the entity's components before removing from the archetype
            auto components = archetype->GetEntityComponents(index);

            // 2. Remove from the archetype so it's no longer visible/usable
            archetype->RemoveEntity(op.entityID);
            entityToArchetype.erase(it);
            EngineSceneManager::GetInstance().getCurrentScene()->RemoveEntity(op.entityID);

            // 3. If the entity had a model or static model, defer destruction
            //    For example, search for EngineModelComponent or EngineStaticModelComponent
            for (auto& comp : components) {
                if (comp->GetType() == typeid(EngineModelComponent)) {
                    auto modelComp = std::dynamic_pointer_cast<EngineModelComponent>(comp);
                    if (modelComp && modelComp->model) {
                        EngineDestructionManager::GetInstance().DeferDestroyModel(
                            modelComp->model,  // <-- pass shared_ptr
                            EngineLoopManager::frame
                        );
                        // optionally: modelComp->model = nullptr if you want
                    }
                }
                // ... or if EngineStaticModelComponent ...
                else if (comp->GetType() == typeid(EngineStaticModelComponent)) {
                    auto staticModelComp = std::dynamic_pointer_cast<EngineStaticModelComponent>(comp);
                    if (staticModelComp && staticModelComp->model) {
                        EngineDestructionManager::GetInstance().DeferDestroyModel(
                            staticModelComp->model, // also shared_ptr
                            EngineLoopManager::frame
                        );
                    }
                }
            }

            EngineDebugManager::log("Removed entity with ID: " + std::to_string(op.entityID), DebugCategory::EntityManager);
        } else {
            // Entity not found
        }
    }

    void EngineEntityManager::ProcessAddComponentToEntity(const EntityOperation& op) {
        auto it = entityToArchetype.find(op.entityID);
        if (it != entityToArchetype.end()) {
            EngineArchetype* oldArchetype = it->second;
            size_t index = oldArchetype->GetEntityIndex(op.entityID);

            // Get existing components
            auto components = oldArchetype->GetEntityComponents(index);
            components.push_back(op.components[0]); // Add the new component

            // Determine new component types
            std::vector<ComponentType> componentTypes;
            for (const auto& component : components) {
                componentTypes.push_back(component->GetType());
            }

            // Recalculate the component signature
            ComponentSignature newSignature = GetComponentSignature(componentTypes);

            // Find or create the new archetype
            EngineArchetype* newArchetype = GetOrCreateArchetype(componentTypes);

            // Move entity to the new archetype
            oldArchetype->RemoveEntity(op.entityID);
            newArchetype->AddEntity(op.entityID, components);

            // Update the mapping
            entityToArchetype[op.entityID] = newArchetype;

            EngineDebugManager::log("Added component to entity with ID: " + std::to_string(op.entityID), DebugCategory::EntityManager);
        } else {
            // Entity not found; handle error if necessary
        }
    }

    void EngineEntityManager::ProcessRemoveComponentFromEntity(const EntityOperation& op) {
        auto it = entityToArchetype.find(op.entityID);
        if (it != entityToArchetype.end()) {
            EngineArchetype* oldArchetype = it->second;
            size_t index = oldArchetype->GetEntityIndex(op.entityID);

            // Get existing components
            auto components = oldArchetype->GetEntityComponents(index);

            // Remove the specified component
            components.erase(std::remove_if(components.begin(), components.end(),
                [&op](const std::shared_ptr<EngineComponent>& comp) {
                    return comp->GetType() == op.componentType;
                }), components.end());

            // Determine new component types
            std::vector<ComponentType> componentTypes;
            for (const auto& component : components) {
                componentTypes.push_back(component->GetType());
            }

            // Recalculate the component signature
            ComponentSignature newSignature = GetComponentSignature(componentTypes);

            // Find or create the new archetype
            EngineArchetype* newArchetype = GetOrCreateArchetype(componentTypes);

            // Move entity to the new archetype
            oldArchetype->RemoveEntity(op.entityID);
            newArchetype->AddEntity(op.entityID, components);

            // Update the mapping
            entityToArchetype[op.entityID] = newArchetype;

            EngineDebugManager::log("Removed component from entity with ID: " + std::to_string(op.entityID), DebugCategory::EntityManager);
        } else {
            // Entity not found; handle error if necessary
        }
    }

    ComponentSignature EngineEntityManager::GetComponentSignature(const std::vector<ComponentType>& componentTypes) {
        ComponentSignature signature;
        // write each component type to log
        
        for (const auto& type : componentTypes) {
            auto it = componentTypeToIndex.find(type);
            if (it != componentTypeToIndex.end()) {
                size_t index = it->second;
                signature.set(index);
            } else {
                // Log the name as well
                EngineDebugManager::log("Component type not registered: " + GetComponentTypeName(type), DebugCategory::EntityManager);
                throw std::runtime_error("Component type not registered");
            }
        }
        return signature;
    }

    std::shared_ptr<EngineComponent> EngineEntityManager::GetComponent(EntityID entityID, ComponentType componentType) {
        auto it = entityToArchetype.find(entityID);
        if (it != entityToArchetype.end()) {
            EngineArchetype* archetype = it->second;
            size_t index = archetype->GetEntityIndex(entityID);
            return archetype->GetComponent(index, componentType);
        }
        return nullptr;
    }
}
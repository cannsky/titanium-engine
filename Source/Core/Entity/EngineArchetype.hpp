#pragma once

#include <vector>
#include <typeindex>
#include <memory>
#include <unordered_map>
#include "EngineEntity.hpp"
#include "EngineComponent.hpp"
#include "EngineTypes.hpp"

namespace TulparEngine {
    class EngineArchetype {
    public:
        // A list of component types that define this archetype
        std::vector<ComponentType> componentTypes;

        ComponentSignature signature;

        EngineArchetype(const std::vector<ComponentType>& types, ComponentSignature signature)
            : componentTypes(types), signature(signature) {}

        // Entities in this archetype
        std::vector<EntityID> entities;

        // Mapping from EntityID to index in entities vector
        std::unordered_map<EntityID, size_t> entityToIndex;

        // Component data arrays, one per component type
        // Map from component type to vector of component pointers
        std::unordered_map<ComponentType, std::vector<std::shared_ptr<EngineComponent>>> components;

        EngineArchetype(const std::vector<ComponentType>& types)
            : componentTypes(types) {}

        // Add an entity and its components to the archetype
        void AddEntity(EntityID entityID, const std::vector<std::shared_ptr<EngineComponent>>& entityComponents) {
            size_t index = entities.size();
            entities.push_back(entityID);
            entityToIndex[entityID] = index;
            for (const auto& component : entityComponents) {
                components[component->GetType()].push_back(component);
            }
        }

        // Remove an entity from the archetype
        void RemoveEntity(EntityID entityID) {
            auto it = entityToIndex.find(entityID);
            if (it != entityToIndex.end()) {
                size_t index = it->second;
                size_t lastIndex = entities.size() - 1;

                if (index != lastIndex) {
                    // Swap entity and components with the last one
                    EntityID lastEntityID = entities[lastIndex];
                    entities[index] = lastEntityID;
                    entityToIndex[lastEntityID] = index;

                    for (auto& [type, compVector] : components) {
                        compVector[index] = compVector[lastIndex];
                    }
                }

                // Remove the last elements
                entities.pop_back();
                entityToIndex.erase(it);

                for (auto& [type, compVector] : components) {
                    compVector.pop_back();
                }
            }
        }

        // Get the index of an entity in the entities vector
        size_t GetEntityIndex(EntityID entityID) const {
            auto it = entityToIndex.find(entityID);
            if (it != entityToIndex.end()) {
                return it->second;
            } else {
                throw std::runtime_error("Entity not found in archetype");
            }
        }

        // Get components of an entity by index
        std::vector<std::shared_ptr<EngineComponent>> GetEntityComponents(size_t index) const {
            std::vector<std::shared_ptr<EngineComponent>> entityComponents;
            for (const auto& type : componentTypes) {
                const auto& compVector = components.at(type);
                entityComponents.push_back(compVector[index]);
            }
            return entityComponents;
        }

        // Get a specific component type for an entity by index
        std::shared_ptr<EngineComponent> GetComponent(size_t index, ComponentType componentType) const {
            auto it = components.find(componentType);
            if (it != components.end()) {
                const auto& compVector = it->second;
                if (index < compVector.size()) {
                    return compVector[index];
                } else {
                    throw std::out_of_range("Index out of range for component vector");
                }
            } else {
                throw std::invalid_argument("Component type not found in archetype");
            }
        }

        inline EntityID GetEntityID(size_t index) const {
            if (index >= entities.size()) {
                throw std::out_of_range("Entity index out of range in EngineArchetype::GetEntityID");
            }
            return entities[index];
        }

        template<typename ComponentType>
        ComponentType* GetComponent(size_t index) {
            auto it = components.find(typeid(ComponentType));
            if (it != components.end()) {
                auto& compVector = it->second;
                if (index < compVector.size()) {
                    return static_cast<ComponentType*>(compVector[index].get());
                }
            }
            return nullptr;
        }
    };
}
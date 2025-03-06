#include "PhysicsSystemManager.hpp"
#include <algorithm>
#include <thread>

namespace TulparEngine::Physics {

    void PhysicsSystemManager::Update() {
        // We get all archetypes from the EngineEntityManager
        // and run only the physics systems on those archetypes that match.

        auto& entityManager = TulparEngine::EngineEntityManager::GetInstance();
        auto& archetypes = entityManager.GetArchetypes();

        for (auto& systemPtr : physicsSystems) {
            EngineSystem& system = *systemPtr;
            // Get the signature for this system
            auto systemComponentTypes = system.GetComponentTypes();
            ComponentSignature systemSignature = entityManager.GetComponentSignature(systemComponentTypes);

            for (auto& [signature, archetypePtr] : archetypes) {
                EngineArchetype& archetype = *archetypePtr;
                // Check if archetype matches system
                if ((archetype.signature & systemSignature) == systemSignature) {
                    RunSystemOnArchetype(system, archetype);
                }
            }
        }
    }

    void PhysicsSystemManager::RunSystemOnArchetype(EngineSystem& system, EngineArchetype& archetype) {
        // Define batch size as per your needs
        const size_t batchSize = 2; 
        size_t numEntities = archetype.entities.size();
        size_t numBatches = (numEntities + batchSize - 1) / batchSize;

        std::vector<std::thread> threads;
        threads.reserve(numBatches);

        for (size_t batch = 0; batch < numBatches; ++batch) {
            size_t startIdx = batch * batchSize;
            size_t endIdx = std::min(startIdx + batchSize, numEntities);

            threads.emplace_back([&system, &archetype, startIdx, endIdx]() {
                system.UpdateBatch(archetype, startIdx, endIdx);
            });
        }

        for (auto& t : threads) {
            t.join();
        }
    }

}
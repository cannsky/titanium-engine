#include "EngineDestructionManager.hpp"
#include "EngineLoopManager.hpp"
#include "EngineDebugManager.hpp"

namespace TulparEngine {

    void EngineDestructionManager::DeferDestroyModel(std::shared_ptr<EngineModel> model, int frameIndexWhenSafe) {
        std::lock_guard<std::mutex> lock(destructionMutex);
        PendingDestruction pd;
        pd.modelPtr = model;
        pd.staticModelPtr = nullptr;
        pd.safeFrameIndex = frameIndexWhenSafe;
        pendingModels.push_back(std::move(pd));
    }

    void EngineDestructionManager::DeferDestroyModel(std::shared_ptr<EngineStaticModel> model, int frameIndexWhenSafe) {
        std::lock_guard<std::mutex> lock(destructionMutex);
        PendingDestruction pd;
        pd.modelPtr = nullptr;
        pd.staticModelPtr = model;
        pd.safeFrameIndex = frameIndexWhenSafe;
        pendingModels.push_back(std::move(pd));
    }

    void EngineDestructionManager::Run() {
        // 1. Lock while we filter out what's safe to destroy
        std::lock_guard<std::mutex> lock(destructionMutex);

        int currentFrame = EngineLoopManager::frame;

        // We'll keep items that aren't yet safe
        std::vector<PendingDestruction> stillPending;
        stillPending.reserve(pendingModels.size());

        for (auto &pd : pendingModels) {
            // We wait 2 frames after scheduling
            if (currentFrame >= pd.safeFrameIndex + 2) {
                // Freed automatically when shared_ptr refcount == 1
                EngineDebugManager::log("Deferred destruction finalized (frame = " 
                    + std::to_string(currentFrame) + ")", 
                    DebugCategory::EntityManager);
            } else {
                // Not yet safe => keep it in the queue
                stillPending.push_back(std::move(pd));
            }
        }

        pendingModels.swap(stillPending);
    }
}
#pragma once

#include <memory>
#include <vector>
#include <mutex>

#include "EngineModel.hpp"
#include "EngineStaticModel.hpp"

namespace TulparEngine {

    struct PendingDestruction {
        // We store either a dynamic model or a static model, whichever is relevant
        std::shared_ptr<EngineModel> modelPtr;
        std::shared_ptr<EngineStaticModel> staticModelPtr;
        
        int safeFrameIndex; // the CPU frame at which we scheduled destruction
    };

    class EngineDestructionManager {
    public:
        static EngineDestructionManager& GetInstance() {
            static EngineDestructionManager instance;
            return instance;
        }

        // Schedules a dynamic model to be destroyed
        void DeferDestroyModel(std::shared_ptr<EngineModel> model, int frameIndexWhenSafe);

        // Schedules a static model to be destroyed
        void DeferDestroyModel(std::shared_ptr<EngineStaticModel> model, int frameIndexWhenSafe);

        void Init() {}

        // Called each frame from EngineLoopManager::run() to actually free objects
        void Run();

    private:
        EngineDestructionManager() = default;
        ~EngineDestructionManager() = default;

        std::vector<PendingDestruction> pendingModels;
        std::mutex destructionMutex;
    };
}
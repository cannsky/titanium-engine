#include "EngineAIAgentSystem.hpp"
#include "EngineRequestManager.hpp"
#include "EngineDebugManager.hpp"

namespace TulparEngine {
    void EngineAIAgentSystem::UpdateBatch(EngineArchetype& archetype, size_t startIdx, size_t endIdx) {
        float deltaTime = EngineLoopManager::getDeltaTime();

        for (size_t i = startIdx; i < endIdx; ++i) {
            auto aiAgentComponent = std::static_pointer_cast<EngineAIAgentComponent>(
                archetype.GetComponent(i, typeid(EngineAIAgentComponent)));

            if (aiAgentComponent->isProcessing && !aiAgentComponent->isDone) {
                continue;
            }

            aiAgentComponent->isProcessing = true;

            Tools::EngineRequestManager::GetInstance().apiRequestAsyncWithCallback(
                "http://localhost:11434/api/generate",
                "llama3.2:1b-instruct-q2_K",
                aiAgentComponent->input,
                [aiAgentComponentWeak = std::weak_ptr<EngineAIAgentComponent>(aiAgentComponent)](const LLMResponse& response) {
                    // This lambda runs on the main thread after ProcessCallbacks is called
                    if (auto aiAgentComponent = aiAgentComponentWeak.lock()) {
                        aiAgentComponent->response = true;
                        aiAgentComponent->isProcessing = false;

                        EngineDebugManager::log(response.response, DebugCategory::AI);
                    }
                }
            );
        }
    }

    REGISTER_SYSTEM(EngineAIAgentSystem)
}
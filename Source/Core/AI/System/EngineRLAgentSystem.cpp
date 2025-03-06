#include "EngineRLAgentSystem.hpp"
#include "EngineRLAgentComponent.hpp"
#include "EngineTransformComponent.hpp"
#include "EngineRequestManagerRL.hpp"
#include "EngineDebugManager.hpp"
#include "EngineLoopManager.hpp"
#include "RLRequest.hpp"
#include "PlayerTargetComponent.hpp"

namespace TulparEngine {
    void EngineRLAgentSystem::UpdateBatch(EngineArchetype& archetype, size_t startIdx, size_t endIdx) {
        float deltaTime = EngineLoopManager::getDeltaTime();

        for (size_t i = startIdx; i < endIdx; ++i) {
            auto rlAgentComponent = std::static_pointer_cast<EngineRLAgentComponent>(
                archetype.GetComponent(i, typeid(EngineRLAgentComponent)));

            auto transformComponent = std::static_pointer_cast<EngineTransformComponent>(
                archetype.GetComponent(i, typeid(EngineTransformComponent)));

            auto playerMoveComponent = std::static_pointer_cast<PlayerMoveComponent>(
                archetype.GetComponent(i, typeid(PlayerMoveComponent)));

            auto playerRotateComponent = std::static_pointer_cast<PlayerRotateComponent>(
                archetype.GetComponent(i, typeid(PlayerRotateComponent)));

            auto playerStatsComponent = std::static_pointer_cast<PlayerStatsComponent>(
                archetype.GetComponent(i, typeid(PlayerStatsComponent)));

            auto playerAttackComponent = std::static_pointer_cast<PlayerAttackComponent>(
                archetype.GetComponent(i, typeid(PlayerAttackComponent)));

            auto playerTargetComponent = std::static_pointer_cast<PlayerTargetComponent>(
                archetype.GetComponent(i, typeid(PlayerTargetComponent)));

            auto playerRewardComponent = std::static_pointer_cast<PlayerRewardComponent>(
                archetype.GetComponent(i, typeid(PlayerRewardComponent)));

            auto playerInputComponent = std::static_pointer_cast<PlayerInputComponent>(
                archetype.GetComponent(i, typeid(PlayerInputComponent)));

            // Build RLRequest
            RLRequest requestData;
            requestData.player_x         = transformComponent->translation.x;
            requestData.player_z         = transformComponent->translation.z;
            requestData.move_speed       = playerMoveComponent->speed;
            requestData.rot_x            = transformComponent->rotation.x;
            requestData.rot_speed        = playerRotateComponent->speed;
            requestData.player_health    = playerStatsComponent->health;
            requestData.attack_cooldown  = playerAttackComponent->currentAttackCooldown;
            requestData.opp_x            = playerTargetComponent->targetX;
            requestData.opp_z            = playerTargetComponent->targetZ;
            requestData.opp_rot          = playerTargetComponent->targetRotX;
            requestData.opp_health       = playerTargetComponent->targetHealth;

            requestData.reward           = playerRewardComponent->reward;
            requestData.done             = false; // set to true if player is "dead" or episode ended

            // Reset the reward for next frame
            playerRewardComponent->reward = 0.0f; 

            // If we haven't requested an action yet, or if we need a new one:
            if (rlAgentComponent->isTraining && !rlAgentComponent->isActionRecieved) {
                // Mark that we're requesting
                rlAgentComponent->isActionRecieved = true;

                // Decide which endpoint to call
                std::string endpoint = "http://localhost:8000/act_player1";
                if (!rlAgentComponent->isPlayerOne) {
                    endpoint = "http://localhost:8000/act_player2";
                }

                // Make async call
                Tools::EngineRequestManagerRL::GetInstance().rlRequestAsyncWithCallback(
                    endpoint,
                    requestData,
                    // Capture a weak_ptr to avoid lifetime issues
                    [rlAgentComponentWeak = std::weak_ptr<EngineRLAgentComponent>(rlAgentComponent)]
                    (const RLResponse& response) {
                        if (auto comp = rlAgentComponentWeak.lock()) {
                            if (comp == nullptr) return;
                            // We got a response
                            comp->isActionRecieved = false; 
                            comp->chosenAction     = response.action;
                            
                            EngineDebugManager::log(
                                "RL Action = " + std::to_string(response.action) +
                                " | msg = " + response.msg,
                                DebugCategory::AI
                            );
                        }
                    }
                );
            }

            // Once callback arrives, the chosenAction is set. Another system can read `chosenAction`
            // and drive the player's movement/rotation/attack.
        }
    }
}
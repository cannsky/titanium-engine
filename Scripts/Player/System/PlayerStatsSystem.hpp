#pragma once

#include "EngineSystem.hpp"
#include "EngineSystemRegistration.hpp"
#include "EngineEntityManager.hpp"
#include "EngineDebugManager.hpp"
#include "EngineLoopManager.hpp"
#include "PlayerStatsComponent.hpp"
#include "PlayerTargetComponent.hpp"
#include "PlayerRewardComponent.hpp"

namespace TulparEngine::Examples::SimpleShooter {
    // PlayerMoveSystem derived from EngineSystem
    class PlayerStatsSystem : public EngineSystem {
    public:
        virtual void UpdateBatch(EngineArchetype& archetype, size_t startIdx, size_t endIdx) override {
            for (size_t i = startIdx; i < endIdx; ++i) {

                auto statsComponent = std::static_pointer_cast<PlayerStatsComponent>(
                    archetype.GetComponent(i, typeid(PlayerStatsComponent)));

                auto targetComponent = std::static_pointer_cast<PlayerTargetComponent>(
                    archetype.GetComponent(i, typeid(PlayerTargetComponent)));

                auto rewardComponent = std::static_pointer_cast<PlayerRewardComponent>(
                    archetype.GetComponent(i, typeid(PlayerRewardComponent)));
                
                if (statsComponent->health <= 0 && !statsComponent->isDead) {
                    statsComponent->isDead = true;

                    if (targetComponent->targetEntityID > 0) {
                        auto targetRewardComponent = std::static_pointer_cast<PlayerRewardComponent>(
                            EngineEntityManager::GetInstance().GetComponent(targetComponent->targetEntityID, typeid(PlayerRewardComponent)));
                        
                        rewardComponent->reward -= rewardComponent->takeDownPenalty;
                        targetRewardComponent->reward += targetRewardComponent->takeDownReward;

                        EngineDebugManager::log("Player:" + std::to_string(i) + " is dead", DebugCategory::UserDebug);
                    }
                }

                if (statsComponent->hitCounter > 0) {
                    rewardComponent->reward -= statsComponent->hitCounter * rewardComponent->hitPenalty;
                    statsComponent->hitCounter = 0;

                    // log player reward with id
                    EngineDebugManager::log("Reward of Player:" + std::to_string(i) + " " + std::to_string(rewardComponent->reward),DebugCategory::UserDebug);
                    // print health of player
                    EngineDebugManager::log("Health of Player:" + std::to_string(i) + " " + std::to_string(statsComponent->health), DebugCategory::UserDebug);
                }

                if (statsComponent->givenHitCounter > 0) {
                    rewardComponent->reward += statsComponent-> givenHitCounter * rewardComponent->hitReward;
                    statsComponent->givenHitCounter = 0;

                    // log player reward with id
                    EngineDebugManager::log("Reward of Player:" + std::to_string(i) + " " + std::to_string(rewardComponent->reward), DebugCategory::UserDebug);
                }
            }
        }

        virtual const std::vector<ComponentType>& GetComponentTypes() const override {
            static std::vector<ComponentType> types = {
                typeid(PlayerStatsComponent),
                typeid(PlayerTargetComponent),
                typeid(PlayerRewardComponent)
            };
            return types;
        }
    };

    //REGISTER_SYSTEM(PlayerStatsSystem);
}

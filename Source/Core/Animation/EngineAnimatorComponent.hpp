#pragma once

#include "EngineComponent.hpp"
#include "EngineAnimation.hpp"
#include "EngineComponentRegistration.hpp"
#include <string>
#include <unordered_map>
#include <functional>

namespace TulparEngine {
    struct AnimationState {
        std::string name;
        EngineAnimation* animation; // Pointer to the animation clip
    };

    struct AnimationTransition {
        std::string fromState;
        std::string toState;
        std::function<bool()> condition; // Lambda or function pointer for the condition
    };

    class EngineAnimatorComponent : public EngineComponent {
    public:
        EngineAnimatorComponent();

        ComponentType GetType() const override {
            return typeid(EngineAnimatorComponent);
        }

        // Retrieve the current animation
        EngineAnimation* GetCurrentAnimation();
    };
}
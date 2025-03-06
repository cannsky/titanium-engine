#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace TulparEngine {
    struct KeyPosition {
        float time; // Time stamp of the keyframe
        glm::vec3 position; // Position at the keyframe
    };

    struct KeyRotation {
        float time;
        glm::quat rotation; // Rotation at the keyframe
    };

    struct KeyScaling {
        float time;
        glm::vec3 scale; // Scaling at the keyframe
    };

    struct BoneAnimation {
        std::string boneName; // Name of the bone
        std::vector<KeyPosition> positions;
        std::vector<KeyRotation> rotations;
        std::vector<KeyScaling> scalings;
    };

    struct EngineAnimation {
        std::string name; // Name of the animation
        float duration; // Duration of the animation in ticks
        float ticksPerSecond; // Ticks per second
        std::vector<BoneAnimation> boneAnimations; // Channels for each bone
    };
}
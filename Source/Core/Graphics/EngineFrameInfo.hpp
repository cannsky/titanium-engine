#pragma once

#include "EngineCamera.hpp"

#include <vulkan/vulkan.h>

namespace TulparEngine {
    struct FrameInfo {
        int frameIndex;
        float frameTime;
        VkCommandBuffer commandBuffer;
        EngineCamera& camera;
        VkDescriptorSet globalDescriptorSet;
    };
}
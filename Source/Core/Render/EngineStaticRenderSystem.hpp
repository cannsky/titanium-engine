#pragma once

#include "EngineVulkanManager.hpp"
#include "EngineGraphicsPipeline.hpp"
#include "EngineDescriptors.hpp"
#include "EngineFrameInfo.hpp"
#include "EngineStaticModel.hpp"
#include "EngineEntityManager.hpp"
#include "EngineTransformComponent.hpp"
#include "EngineStaticModelComponent.hpp"

#include <memory>
#include <vector>

namespace TulparEngine {
    class EngineStaticRenderSystem {
    public:
        EngineStaticRenderSystem(VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
        ~EngineStaticRenderSystem();

        EngineStaticRenderSystem(const EngineStaticRenderSystem&) = delete;
        EngineStaticRenderSystem& operator=(const EngineStaticRenderSystem&) = delete;

        void renderGameObjects(FrameInfo& frameInfo);

    private:
        void createGraphicsPipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void createGraphicsPipeline(VkRenderPass renderPass);

        VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
        std::unique_ptr<EngineGraphicsPipeline> graphicsPipeline;
    };
}
#pragma once

#include "EngineVulkanManager.hpp"
#include "EngineGraphicsPipeline.hpp"
#include "EngineRenderer.hpp"
#include "EngineWindowManager.hpp"
#include "EngineCamera.hpp"
#include "EngineFrameInfo.hpp"

#include "EngineEntityManager.hpp"
#include "EngineTransformComponent.hpp"
#include "EngineModelComponent.hpp"
#include "EngineEntity.hpp"

namespace TulparEngine {
    class EngineRenderSystem {
    public:
        EngineRenderSystem(VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
        ~EngineRenderSystem();

        // Get the graphics pipeline
        EngineGraphicsPipeline& getGraphicsPipeline() { return *graphicsPipeline; }

        // Deleting copy constructor and assignment operator to enforce singleton
        EngineRenderSystem(const EngineRenderSystem&) = delete;
        void operator=(const EngineRenderSystem&) = delete;

        void renderGameObjects(FrameInfo& frameInfo);
    private:
        void createGraphicsPipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void createSwapChain();
        void createGraphicsPipeline(VkRenderPass renderPass);

        std::unique_ptr<EngineGraphicsPipeline> graphicsPipeline;
        VkPipelineLayout pipelineLayout;
    };
}    
#include "EngineRenderSystem.hpp"

namespace TulparEngine {
    EngineRenderSystem::EngineRenderSystem(VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout) {
        createGraphicsPipelineLayout(globalSetLayout);
        createGraphicsPipeline(renderPass);
    }

    EngineRenderSystem::~EngineRenderSystem() { 
        vkDestroyPipelineLayout(EngineVulkanManager::GetInstance().getDevice(), pipelineLayout, nullptr);
    }

    void EngineRenderSystem::createGraphicsPipelineLayout(VkDescriptorSetLayout globalSetLayout) {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstantData);

        VkDescriptorSetLayout textureSetLayout = EngineVulkanManager::GetInstance().getTextureDescriptorSetLayout();
        VkDescriptorSetLayout bonesSetLayout = EngineVulkanManager::GetInstance().getBonesDescriptorSetLayout();

        std::vector<VkDescriptorSetLayout> descriptorSetLayouts{globalSetLayout, textureSetLayout, bonesSetLayout};

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
        pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
        if (vkCreatePipelineLayout(EngineVulkanManager::GetInstance().getDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }

    void EngineRenderSystem::createGraphicsPipeline(VkRenderPass renderPass) {
        assert(pipelineLayout != nullptr && "Pipeline layout is not created!");
        PipelineConfigInfo pipelineConfig{};
        // Create the graphics pipeline layout
        EngineGraphicsPipeline::defaultPipelineConfigInfo(pipelineConfig);
        // Set render pass
        pipelineConfig.renderPass = renderPass;
        // Set pipeline layout
        pipelineConfig.pipelineLayout = pipelineLayout;
        // Set vertex input descriptions
        pipelineConfig.bindingDescriptions = EngineModel::Vertex::getBindingDescriptions();
        pipelineConfig.attributeDescriptions = EngineModel::Vertex::getAttributeDescriptions();
        // Set the pipeline config render pass
        graphicsPipeline = EngineGraphicsPipeline::create("Resources/Shaders/ModelShader.vert.spv", "Resources/Shaders/ModelShader.frag.spv", pipelineConfig);
    }

    void EngineRenderSystem::renderGameObjects(FrameInfo& frameInfo) {
        graphicsPipeline->bind(frameInfo.commandBuffer);

        vkCmdBindDescriptorSets(
            frameInfo.commandBuffer, 
            VK_PIPELINE_BIND_POINT_GRAPHICS, 
            pipelineLayout, 
            0, 
            1, 
            &frameInfo.globalDescriptorSet, 
            0, 
            nullptr
        );

        EngineEntityManager::GetInstance().each<EngineTransformComponent, EngineModelComponent>(
            [&](EngineEntity entity, EngineTransformComponent& transform, EngineModelComponent& modelComponent) {
                /*SimplePushConstantData push{};
                push.modelMatrix = transform.mat4();
                push.normalMatrix = transform.normalMatrix();
                vkCmdPushConstants(
                    frameInfo.commandBuffer, 
                    pipelineLayout, 
                    VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 
                    0, 
                    sizeof(SimplePushConstantData), 
                    &push
                );*/

                modelComponent.model->UpdatePushConstantsWithScaling(frameInfo.commandBuffer, pipelineLayout, transform);

                // Bind per-model descriptor sets (texture and bones)
                std::array<VkDescriptorSet, 2> descriptorSets{};
                descriptorSets[0] = modelComponent.model->getTextureDescriptorSet(); // set = 1
                descriptorSets[1] = modelComponent.model->getBonesDescriptorSet();   // set = 2

                vkCmdBindDescriptorSets(
                    frameInfo.commandBuffer,
                    VK_PIPELINE_BIND_POINT_GRAPHICS,
                    pipelineLayout,
                    1, // First set index for per-model descriptors
                    static_cast<uint32_t>(descriptorSets.size()),
                    descriptorSets.data(),
                    0,
                    nullptr
                );

                modelComponent.model->bind(frameInfo.commandBuffer);
                modelComponent.model->draw(frameInfo.commandBuffer);
            }
        );
    }
}
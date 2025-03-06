#include "EngineStaticRenderSystem.hpp"
#include "EngineDebugManager.hpp"
#include "EngineTexture.hpp"
#include <glm/mat4x4.hpp>

namespace TulparEngine {
    EngineStaticRenderSystem::EngineStaticRenderSystem(VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout) {
        createGraphicsPipelineLayout(globalSetLayout);
        createGraphicsPipeline(renderPass);
    }

    EngineStaticRenderSystem::~EngineStaticRenderSystem() {
        if (pipelineLayout != VK_NULL_HANDLE) {
            vkDestroyPipelineLayout(EngineVulkanManager::GetInstance().getDevice(), pipelineLayout, nullptr);
        }
    }

    void EngineStaticRenderSystem::createGraphicsPipelineLayout(VkDescriptorSetLayout globalSetLayout) {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(glm::mat4) * 2; // Assuming modelMatrix + normalMatrix

        VkDescriptorSetLayout textureSetLayout = EngineVulkanManager::GetInstance().getTextureDescriptorSetLayout();

        std::vector<VkDescriptorSetLayout> descriptorSetLayouts{ globalSetLayout, textureSetLayout };

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.pNext = nullptr;
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
        pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

        if (vkCreatePipelineLayout(EngineVulkanManager::GetInstance().getDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout for static render system!");
        }
    }

    void EngineStaticRenderSystem::createGraphicsPipeline(VkRenderPass renderPass) {
        assert(pipelineLayout != nullptr && "Cannot create pipeline before layout!");

        PipelineConfigInfo pipelineConfig{};
        EngineGraphicsPipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = pipelineLayout;

        pipelineConfig.bindingDescriptions = EngineStaticModel::Vertex::getBindingDescriptions();
        pipelineConfig.attributeDescriptions = EngineStaticModel::Vertex::getAttributeDescriptions();

        const std::string vertShaderPath = "Resources/Shaders/StaticModelShader.vert.spv";
        const std::string fragShaderPath = "Resources/Shaders/StaticModelShader.frag.spv";

        graphicsPipeline = EngineGraphicsPipeline::create(vertShaderPath, fragShaderPath, pipelineConfig);
    }

    void EngineStaticRenderSystem::renderGameObjects(FrameInfo& frameInfo) {
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

        EngineEntityManager::GetInstance().each<EngineTransformComponent, EngineStaticModelComponent>(
            [&](EngineEntity entity, EngineTransformComponent& transform, EngineStaticModelComponent& modelComponent) {
                modelComponent.model->UpdatePushConstantsWithScaling(frameInfo.commandBuffer, pipelineLayout, transform);

                VkDescriptorSet textureSet = modelComponent.model->getTextureDescriptorSet();
                vkCmdBindDescriptorSets(
                    frameInfo.commandBuffer,
                    VK_PIPELINE_BIND_POINT_GRAPHICS,
                    pipelineLayout,
                    1,
                    1,
                    &textureSet,
                    0,
                    nullptr
                );

                modelComponent.model->bind(frameInfo.commandBuffer);
                modelComponent.model->draw(frameInfo.commandBuffer);
            }
        );
    }
}
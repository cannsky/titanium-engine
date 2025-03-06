#pragma once

#include <string>
#include <iostream>
#include <vector>
#include "EngineFileManager.hpp"
#include "EngineDebugManager.hpp"
#include "EngineVulkanManager.hpp"
#include "EngineModel.hpp"

namespace TulparEngine {
    struct PipelineConfigInfo {
        PipelineConfigInfo() = default;

        PipelineConfigInfo(const PipelineConfigInfo&) = delete;
        PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

        VkPipelineViewportStateCreateInfo viewportInfo;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        std::vector<VkDynamicState> dynamicStateEnables;
        VkPipelineDynamicStateCreateInfo dynamicStateInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
        std::vector<VkVertexInputBindingDescription> bindingDescriptions{};
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};
    };
    class EngineGraphicsPipeline {
    public:
        EngineGraphicsPipeline();
        ~EngineGraphicsPipeline();

        // Initialize all systems
        void Init();
        // Run the main engine loop
        void run();
        // Cleanup all systems
        void cleanup();
        // Create the graphics pipeline
        static std::unique_ptr<EngineGraphicsPipeline> create(const std::string& vertShaderPath, const std::string& fragShaderPath, const PipelineConfigInfo& configInfo);
        static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

        void createGraphicsPipeline(const std::string& vertShaderPath, const std::string& fragShaderPath, const PipelineConfigInfo& configInfo);

        void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

        void bind(VkCommandBuffer commandBuffer);

        // Deleting copy constructor and assignment operator to enforce singleton
        EngineGraphicsPipeline(const EngineGraphicsPipeline&) = delete;
        EngineGraphicsPipeline& operator=(const EngineGraphicsPipeline&) = delete;
    private:
        VkPipeline graphicsPipeline;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;
    };
}
#include "EngineRenderer.hpp"

namespace TulparEngine {

    EngineRenderer::EngineRenderer() {
        // Recreate the swap chain
        recreateSwapChain();
        // Create command buffers
        createCommandBuffers();
    }

    EngineRenderer::~EngineRenderer() { 
        // Wait until the device is idle before cleanup
        vkDeviceWaitIdle(EngineVulkanManager::GetInstance().getDevice());
        // Free command buffers
        freeCommandBuffers();
    }
    
    void EngineRenderer::createCommandBuffers() {
        commandBuffers.resize(EngineSwapChain::MAX_FRAMES_IN_FLIGHT);
        VkCommandBufferAllocateInfo allocInfo{};

        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = EngineVulkanManager::GetInstance().getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(EngineVulkanManager::GetInstance().getDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }
    }

    void EngineRenderer::recreateSwapChain() {
        vkDeviceWaitIdle(EngineVulkanManager::GetInstance().getDevice());
        auto extent = EngineWindowManager::getExtent();
        while (extent.width == 0 || extent.height == 0) {
            extent = EngineWindowManager::getExtent();
            glfwWaitEvents();
        }

        if (swapChain == nullptr) {
            swapChain = EngineSwapChain::create();
        }
        else {
            // Create a shared_ptr from the unique_ptr
            std::shared_ptr<EngineSwapChain> oldSwapChain = std::move(swapChain);
            swapChain = EngineSwapChain::create(oldSwapChain);

            if (!oldSwapChain->compareSwapFormats(*swapChain.get())) {
                throw std::runtime_error("Swap chain image format or extent changed!");
            }
        }

        
    }

    void EngineRenderer::freeCommandBuffers() {
        vkFreeCommandBuffers(EngineVulkanManager::GetInstance().getDevice(), EngineVulkanManager::GetInstance().getCommandPool(), static_cast<float>(commandBuffers.size()), commandBuffers.data());
        commandBuffers.clear();
    }

    VkCommandBuffer EngineRenderer::beginFrame() {
        if (isFrameStarted) {
            throw std::runtime_error("Called beginFrame while frame already in progress.");
        }

        auto result = swapChain->acquireNextImage(&currentImageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return nullptr;
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("Failed to acquire next image!");
        }

        isFrameStarted = true;

        auto commandBuffer = getCurrentCommandBuffer();

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        return commandBuffer;
    }

    void EngineRenderer::endFrame() {
        if (!isFrameStarted) {
            throw std::runtime_error("Called endFrame while frame not in progress.");
        }

        auto commandBuffer = getCurrentCommandBuffer();

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }

        auto result = swapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || EngineWindowManager::GetInstance().wasWindowResized()) {
            EngineWindowManager::GetInstance().resetWindowResizedFlag();
            recreateSwapChain();
        } else if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to present swap chain image!");
        }

        isFrameStarted = false;
        currentFrameIndex = (currentFrameIndex + 1) % EngineSwapChain::MAX_FRAMES_IN_FLIGHT;
    }

    void EngineRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer) {
        assert(isFrameStarted && "Cannot begin render pass when frame not in progress.");
        assert(commandBuffer == getCurrentCommandBuffer() && "Can only begin render pass for active command buffer.");
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = swapChain->getRenderPass();
        renderPassInfo.framebuffer = swapChain->getFrameBuffer(currentImageIndex);
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(swapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(swapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor({0, 0}, swapChain->getSwapChainExtent());
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    }

    void EngineRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer) {
        assert(isFrameStarted && "Cannot end render pass when frame not in progress.");
        assert(commandBuffer == getCurrentCommandBuffer() && "Can only end render pass for active command buffer.");
        vkCmdEndRenderPass(commandBuffer);
    }
}
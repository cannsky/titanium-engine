#pragma once

#include "EngineVulkanManager.hpp"
#include <memory>
#include <string>

namespace TulparEngine {
    class EngineTexture {
    public:
        EngineTexture() = default;
        ~EngineTexture();
        static std::shared_ptr<EngineTexture> createTextureFromFile(const std::string &filePath);
        VkDescriptorImageInfo getDescriptorImageInfo() const;

    private:
        VkImage textureImage;
        VkDeviceMemory textureImageMemory;
        VkImageView textureImageView;
        VkSampler textureSampler;
    };
}
#include "EngineBuffer.hpp"

#include <cassert>
#include <cstring>

namespace TulparEngine {

    VkDeviceSize EngineBuffer::getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment) {
        if (minOffsetAlignment > 0) {
            return (instanceSize + minOffsetAlignment - 1) & ~(minOffsetAlignment - 1);
        }
        return instanceSize;
        }
        
        EngineBuffer::EngineBuffer(
            VkDeviceSize instanceSize,
            uint32_t instanceCount,
            VkBufferUsageFlags usageFlags,
            VkMemoryPropertyFlags memoryPropertyFlags,
            VkDeviceSize minOffsetAlignment)
            : instanceSize{instanceSize},
            instanceCount{instanceCount},
            usageFlags{usageFlags},
            memoryPropertyFlags{memoryPropertyFlags} {
        alignmentSize = getAlignment(instanceSize, minOffsetAlignment);
        bufferSize = alignmentSize * instanceCount;
        EngineVulkanManager::GetInstance().createBuffer(bufferSize, usageFlags, memoryPropertyFlags, buffer, memory);
    }
    
    EngineBuffer::~EngineBuffer() {
        unmap();
        vkDestroyBuffer(EngineVulkanManager::GetInstance().getDevice(), buffer, nullptr);
        vkFreeMemory(EngineVulkanManager::GetInstance().getDevice(), memory, nullptr);
    }
        
    VkResult EngineBuffer::map(VkDeviceSize size, VkDeviceSize offset) {
        assert(buffer && memory && "Called map on buffer before create");
        return vkMapMemory(EngineVulkanManager::GetInstance().getDevice(), memory, offset, size, 0, &mapped);
    }
    
    void EngineBuffer::unmap() {
        if (mapped) {
            vkUnmapMemory(EngineVulkanManager::GetInstance().getDevice(), memory);
            mapped = nullptr;
        }
    }
    
    void EngineBuffer::writeToBuffer(void *data, VkDeviceSize size, VkDeviceSize offset) {
        assert(mapped && "Cannot copy to unmapped buffer");
        
        if (size == VK_WHOLE_SIZE) {
            memcpy(mapped, data, bufferSize);
        } else {
            char *memOffset = (char *)mapped;
            memOffset += offset;
            memcpy(memOffset, data, size);
        }
    }
    
    VkResult EngineBuffer::flush(VkDeviceSize size, VkDeviceSize offset) {
        VkMappedMemoryRange mappedRange = {};
        mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        mappedRange.memory = memory;
        mappedRange.offset = offset;
        mappedRange.size = size;
        return vkFlushMappedMemoryRanges(EngineVulkanManager::GetInstance().getDevice(), 1, &mappedRange);
    }
    
    VkResult EngineBuffer::invalidate(VkDeviceSize size, VkDeviceSize offset) {
        VkMappedMemoryRange mappedRange = {};
        mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        mappedRange.memory = memory;
        mappedRange.offset = offset;
        mappedRange.size = size;
        return vkInvalidateMappedMemoryRanges(EngineVulkanManager::GetInstance().getDevice(), 1, &mappedRange);
    }
    
    VkDescriptorBufferInfo EngineBuffer::descriptorInfo(VkDeviceSize size, VkDeviceSize offset) {
        return VkDescriptorBufferInfo{
            buffer,
            offset,
            size,
        };
    }
    
    void EngineBuffer::writeToIndex(void *data, int index) {
        writeToBuffer(data, instanceSize, index * alignmentSize);
    }
    
    VkResult EngineBuffer::flushIndex(int index) { return flush(alignmentSize, index * alignmentSize); }
    
    VkDescriptorBufferInfo EngineBuffer::descriptorInfoForIndex(int index) {
        return descriptorInfo(alignmentSize, index * alignmentSize);
    }
    
    VkResult EngineBuffer::invalidateIndex(int index) {
        return invalidate(alignmentSize, index * alignmentSize);
    }
 
}
#include "EditorUIManager.hpp"
#include "NodeEditorManager.hpp"
#include "NodeEditorUI.hpp" // Include the NodeEditorUI header

namespace TulparEngine::EngineEditor {

    EditorUIManager::EditorUIManager() {}

    EditorUIManager::~EditorUIManager() { cleanup(); }

    void EditorUIManager::Init() {
        setupGui();
    }

    void EditorUIManager::start() {
        
    }

    void EditorUIManager::run() {
        
    }

    void EditorUIManager::cleanup() {

    }

    void EditorUIManager::setupGui() {
        // 1. Initialize ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImNodes::CreateContext();
        ImGui::StyleColorsDark();

        // 2. Initialize ImGui for GLFW
        ImGui_ImplGlfw_InitForVulkan(EngineWindowManager::GetInstance().getWindowPointer(), true);

        // 3. Create Descriptor Pool for ImGui
        VkDescriptorPoolSize pool_sizes[] = {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 100 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 100 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 100 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 100 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 100 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 100 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 100 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 100 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 100 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 100 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 100 }
        };
        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 100 * IM_ARRAYSIZE(pool_sizes);
        pool_info.poolSizeCount = static_cast<uint32_t>(IM_ARRAYSIZE(pool_sizes));
        pool_info.pPoolSizes = pool_sizes;

        if (vkCreateDescriptorPool(EngineVulkanManager::GetInstance().getDevice(), &pool_info, nullptr, &imguiDescriptorPool) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create ImGui descriptor pool!");
        }

        // 4. ImGui Vulkan Init info
        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = EngineVulkanManager::GetInstance().getVulkanInstance();
        init_info.PhysicalDevice = EngineVulkanManager::GetInstance().getPhysicalDevice();
        init_info.Device = EngineVulkanManager::GetInstance().getDevice();
        init_info.QueueFamily = EngineVulkanManager::GetInstance().findPhysicalQueueFamilies().graphicsFamily;
        init_info.Queue = EngineVulkanManager::GetInstance().getGraphicsQueue();
        init_info.PipelineCache = VK_NULL_HANDLE;
        init_info.DescriptorPool = imguiDescriptorPool;  // Use the new descriptor pool
        init_info.RenderPass = EngineGraphicsManager::GetInstance().getRenderer().getSwapChainRenderPass();
        init_info.MinImageCount = 2;
        init_info.ImageCount = 3;
        init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        init_info.Allocator = nullptr;
        init_info.CheckVkResultFn = [](VkResult result) {
            if (result != VK_SUCCESS) {
                throw std::runtime_error("Vulkan error with ImGui!");
            }
        };

        // Pass your render pass to ImGui initialization
        ImGui_ImplVulkan_Init(&init_info);

        // 5. Upload Fonts
        VkCommandBuffer commandBuffer = EngineVulkanManager::GetInstance().beginSingleTimeCommands();
        ImGui_ImplVulkan_CreateFontsTexture();
        EngineVulkanManager::GetInstance().endSingleTimeCommands(commandBuffer);

        EditorSceneHierarchyUI::GetInstance();
        EditorGameObjectDetailsUI::GetInstance();
        EditorAssistantChatUI::GetInstance();
    }

    void EditorUIManager::renderGui(VkCommandBuffer commandBuffer) {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        topBarUI.renderGui();

        // If the top bar flag is set, show the Node Editor window
        if (topBarUI.showEditorWindow) {
            auto currentEditor = NodeEditorManager::GetInstance().GetCurrentEditor(); // example
            NodeEditorUI::GetInstance().RenderGui(currentEditor);
        }

        EditorSceneHierarchyUI::GetInstance().renderGui();
        EditorGameObjectDetailsUI::GetInstance().renderGui();

        ImGui::Render();
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);
    }

    void EditorUIManager::cleanupGui() {
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        vkDestroyDescriptorPool(EngineVulkanManager::GetInstance().getDevice(), imguiDescriptorPool, nullptr);
    }
}
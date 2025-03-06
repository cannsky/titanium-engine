#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#include "imnodes.h"

#include "EngineWindowManager.hpp"
#include "EngineVulkanManager.hpp"
#include "EngineGraphicsManager.hpp"

#include "EditorTopBarUI.hpp"
#include "EditorSceneHierarchyUI.hpp"
#include "EditorGameObjectDetailsUI.hpp"
#include "EditorAssistantChatUI.hpp"

using namespace TulparEngine;

namespace TulparEngine::EngineEditor {
    class EditorUIManager {
    public:
        // Singleton access method for EditorGuiManager
        static EditorUIManager& GetInstance() {
            static EditorUIManager instance;
            return instance;
        }

        void Init();
        void start();
        void run();
        void cleanup();

        void setupGui();
        void renderGui(VkCommandBuffer commandBuffer);
        void cleanupGui();

        // Deleting copy constructor and assignment operator to enforce singleton
        EditorUIManager(const EditorUIManager&) = delete;
        void operator=(const EditorUIManager&) = delete;
    private:
        EditorUIManager();  // Private constructor for singleton
        ~EditorUIManager();
        VkDescriptorPool imguiDescriptorPool;

        EditorTopBarUI topBarUI;
    };
}
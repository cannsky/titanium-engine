#include "EditorAssistantChatUI.hpp"
#include "EngineBrowserManager.hpp"
#include "EngineVulkanManager.hpp"
#include "EngineDebugManager.hpp"
#include "EditorGameObjectDetailsUI.hpp"
#include <imgui.h>
#ifdef QT_ENABLED
#include <QKeyEvent> // Include this at the top
#endif

namespace TulparEngine::EngineEditor {

    EditorAssistantChatUI::EditorAssistantChatUI() {
        imageSize = ImVec2(400, 800);
    }

    EditorAssistantChatUI::~EditorAssistantChatUI() {
        cleanup();
    }

    void EditorAssistantChatUI::cleanup() {
        // Any cleanup logic if needed
    }

    void EditorAssistantChatUI::renderGui() {
        #ifdef QT_ENABLED
        imageSize = ImVec2(400, EditorGameObjectDetailsUI::GetInstance().panelHeight - 60);
        CheckIsMouseClickingImage();
        VkDescriptorSet descriptorSet = Tools::EngineBrowserManager::GetInstance().GetDescriptorSet();
        if (descriptorSet != VK_NULL_HANDLE) {
            ImGui::Image((ImTextureID)descriptorSet, imageSize);
        }
        #endif
    }

    void EditorAssistantChatUI::CheckIsMouseClickingImage() {
        mousePos = ImGui::GetMousePos();
        imagePos = ImGui::GetCursorScreenPos();

        #ifdef QT_ENABLED
        if (ImGui::IsKeyPressed(ImGuiKey_W)) {
            // 'W' tuşuna basıldığında yapılacak işlemler
            TulparEngine::Tools::EngineBrowserManager::GetInstance().OnKeyPressed(Qt::Key_W);
        }

        isMouseOverImage = (mousePos.x >= imagePos.x && mousePos.x <= (imagePos.x + imageSize.x)) &&
                                (mousePos.y >= imagePos.y && mousePos.y <= (imagePos.y + imageSize.y));

        isMouseClicked = ImGui::IsMouseClicked(0);

        if (isMouseOverImage && isMouseClicked) {
            TulparEngine::Tools::EngineBrowserManager::GetInstance().OnMouseClicked();
        }
        #endif
    }
}
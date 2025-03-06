#include "EditorSceneHierarchyUI.hpp"

namespace TulparEngine::EngineEditor {

    bool showHierarchyPanel = true;  // Controls the visibility of the entire panel
    bool showHierarchyDetails = true;  // Controls whether details are visible

    EditorSceneHierarchyUI::EditorSceneHierarchyUI() {
        // Constructor implementation
    }

    EditorSceneHierarchyUI::~EditorSceneHierarchyUI() {
        // Destructor implementation
    }

    void EditorSceneHierarchyUI::renderGui() {
        if (!showHierarchyPanel) return;

        float topBarHeight = 30.0f;
        ImGui::SetNextWindowPos(ImVec2(0, topBarHeight));
        ImGui::SetNextWindowSize(ImVec2(250, ImGui::GetIO().DisplaySize.y - topBarHeight));

        ImGui::Begin("Scene Hierarchy", &showHierarchyPanel, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

        if (ImGui::Button(showHierarchyDetails ? "Collapse" : "Expand")) {
            showHierarchyDetails = !showHierarchyDetails;
        }

        if (showHierarchyDetails) {
            ImGui::Text("Scene Entities:");

            auto currentScene = EngineSceneManager::GetInstance().getCurrentScene();
            const auto& orderedEntities = currentScene->GetOrderedEntities();

            for (const auto& entityData : orderedEntities) {
                if (ImGui::Selectable(entityData.name.c_str(), selectedEntityID == entityData.entityID)) {
                    selectedEntityID = entityData.entityID;
                }
            }
        }

        ImGui::End();
    }
}
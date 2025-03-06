#include "EditorTopBarUI.hpp"
#include "imgui.h"

namespace TulparEngine::EngineEditor {
    EditorTopBarUI::EditorTopBarUI() {
        // Constructor implementation
    }

    EditorTopBarUI::~EditorTopBarUI() {
        // Destructor implementation
    }

    void EditorTopBarUI::renderGui() {
        // Set position and size for the top bar UI
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, 30));

        // Render the top bar as an always-on-top window
        ImGui::Begin("Top Bar", nullptr, 
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
                     ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        float buttonSpacing = 15.0f;
        ImGui::SetCursorPosX(10);

        if (ImGui::Button("Button 1")) { /* Action for Button 1 */ }
        ImGui::SameLine(0, buttonSpacing);
        
        // Changed button label here:
        if (ImGui::Button("Open Node Editor")) { 
            // Toggle the editor window visibility
            showEditorWindow = !showEditorWindow;
        }
        
        ImGui::SameLine(0, buttonSpacing);
        if (ImGui::Button("Button 3")) { /* Action for Button 3 */ }
        ImGui::SameLine(0, buttonSpacing);
        if (ImGui::Button("Button 4")) { /* Action for Button 4 */ }
        ImGui::SameLine(0, buttonSpacing);
        if (ImGui::Button("Button 5")) { /* Action for Button 5 */ }

        ImGui::End();
    }
}
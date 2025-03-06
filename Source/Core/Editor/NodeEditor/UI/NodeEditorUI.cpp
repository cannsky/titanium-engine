#include "NodeEditorUI.hpp"
#include "BaseNodeEditor.hpp"
#include "VisualScriptEditor.hpp"
#include "StateMachineEditor.hpp"
#include "NodeEditorLeftPanelUI.hpp"
#include "NodeEditorManager.hpp"
#include "VSNodeRenderers.hpp"
#include "SMNodeRenderers.hpp"
#include <imgui.h>
#include <imnodes.h>

namespace TulparEngine::EngineEditor {

    NodeEditorUI& NodeEditorUI::GetInstance() {
        static NodeEditorUI instance;
        return instance;
    }

    NodeEditorUI::NodeEditorUI() {
        // Initialize ImNodes or other UI systems if necessary
        RegisterVSNodeRenderers();
        RegisterSMNodeRenderers();
    }

    NodeEditorUI::~NodeEditorUI() {}

    void NodeEditorUI::RenderGui(BaseNodeEditor* currentEditor) {
        // This creates a window with a menu bar (like your old code)
        if (ImGui::Begin("Node Editor", nullptr, ImGuiWindowFlags_MenuBar)) {
            ShowMainMenuBar();

            // If you have dialog flags, you can add them here just like your old code:
            // if (newVisualScriptDialog) ShowCreateVisualScriptDialog();
            // if (newStateMachineDialog) ShowCreateStateMachineDialog();
            // if (loadDialog) ShowLoadDialog();
            // if (saveDialog) ShowSaveDialog();
            // if (newFunctionDialog) ShowCreateFunctionDialog();

            if (!currentEditor) {
                ImGui::Text("No editor loaded. Create or load one from the 'File' menu.");
            } else {
                ImGui::Columns(2, "EditorColumns");
                ImGui::SetColumnWidth(0, ImGui::GetWindowWidth() * 0.7f);

                if (auto vsEditor = dynamic_cast<VisualScriptEditor*>(currentEditor)) {
                    RenderVisualScriptUI(vsEditor);
                } else if (auto smEditor = dynamic_cast<StateMachineEditor*>(currentEditor)) {
                    RenderStateMachineUI(smEditor);
                } else {
                    ImGui::Text("Unknown editor type loaded.");
                }

                ImGui::NextColumn();

                NodeEditorLeftPanelUI::GetInstance().Render(currentEditor);

                ImGui::Columns(1);
            }
        }
        ImGui::End();
    }

    void NodeEditorUI::ShowMainMenuBar() {
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New Visual Script")) {
                    NodeEditorManager::GetInstance().CreateNewVisualScriptEditor("NewScript");
                }
                if (ImGui::MenuItem("New State Machine")) {
                    NodeEditorManager::GetInstance().CreateNewStateMachineEditor("NewStateMachine");
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Load Visual Script")) {
                    std::string filePath = "...";
                    NodeEditorManager::GetInstance().LoadEditor(filePath, true);
                }
                if (ImGui::MenuItem("Load State Machine")) {
                    std::string filePath = "...";
                    NodeEditorManager::GetInstance().LoadEditor(filePath, false);
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
    }

    void NodeEditorUI::RenderVisualScriptUI(BaseNodeEditor* vsEditor) {
        ImNodes::BeginNodeEditor();

        auto& nodes = vsEditor->GetNodes();
        for (auto& [id, node] : nodes) {
            ImNodes::SetNodeEditorSpacePos(id, node.position);

            ImNodes::BeginNode(id);
            ImNodes::BeginNodeTitleBar();
            ImGui::TextUnformatted(node.name.c_str());
            ImNodes::EndNodeTitleBar();

            auto renderer = VSNodeRendererRegistry::Instance().GetRenderer(node.vsType);
            if (renderer) {
                renderer->RenderNode(node);
            }

            ImNodes::EndNode();
        }

        // Render links
        for (auto& link : vsEditor->GetLinks()) {
            ImNodes::Link(link.id, link.startAttribute, link.endAttribute);
        }

        ImNodes::EndNodeEditor();
        vsEditor->UpdateNodePositions();
    }

    void NodeEditorUI::RenderStateMachineUI(BaseNodeEditor* smEditor) {
        ImNodes::BeginNodeEditor();

        auto& nodes = smEditor->GetNodes();
        for (auto& [id, node] : nodes) {
            ImNodes::SetNodeEditorSpacePos(id, node.position);

            ImNodes::BeginNode(id);
            ImNodes::BeginNodeTitleBar();
            ImGui::TextUnformatted(node.name.c_str());
            ImNodes::EndNodeTitleBar();

            auto renderer = SMNodeRendererRegistry::Instance().GetRenderer(StateMachineNodeType::StateNode);
            if (renderer) {
                renderer->RenderNode(node);
            }

            ImNodes::EndNode();
        }

        // Render links
        for (auto& link : smEditor->GetLinks()) {
            ImNodes::Link(link.id, link.startAttribute, link.endAttribute);
        }

        ImNodes::EndNodeEditor();
        smEditor->UpdateNodePositions();
    }

}
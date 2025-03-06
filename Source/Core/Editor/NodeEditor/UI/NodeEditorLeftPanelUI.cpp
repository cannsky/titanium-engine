#include "NodeEditorLeftPanelUI.hpp"
#include "BaseNodeEditor.hpp"
#include "VisualScriptEditor.hpp"
#include "StateMachineEditor.hpp"
#include "GraphTypes.hpp"
#include "GraphManager.hpp"
#include <imgui.h>
#include <cstdio> // for snprintf

namespace TulparEngine::EngineEditor {

    NodeEditorLeftPanelUI& NodeEditorLeftPanelUI::GetInstance() {
        static NodeEditorLeftPanelUI instance;
        return instance;
    }

    void NodeEditorLeftPanelUI::Render(BaseNodeEditor* editor) {
        if (!editor) {
            ImGui::Text("No editor loaded.");
            return;
        }

        // Determine the type of editor
        if (auto vsEditor = dynamic_cast<VisualScriptEditor*>(editor)) {
            RenderVisualScriptPanel(vsEditor);
        } else if (auto smEditor = dynamic_cast<StateMachineEditor*>(editor)) {
            RenderStateMachinePanel(smEditor);
        } else {
            ImGui::Text("Unknown editor type.");
        }
    }

    void NodeEditorLeftPanelUI::RenderVisualScriptPanel(VisualScriptEditor* vsEditor) {
        ImGui::Text("Variables:");
        ImGui::Separator();

        // Get the main graph
        auto* mainGraph = vsEditor->GetMainGraph();
        if (!mainGraph) {
            ImGui::Text("No main graph found.");
            return;
        }

        // Display existing variables in the unified map
        for (auto& kv : mainGraph->variables) {
            const std::string& varName = kv.first;
            EditorVariable& var = kv.second;

            switch (var.type) {
            case VariableType::Bool:
            {
                bool value = std::get<bool>(var.value);
                if (ImGui::Checkbox(varName.c_str(), &value)) {
                    var.value = value; 
                }
                break;
            }
            case VariableType::Int:
            {
                int value = std::get<int>(var.value);
                if (ImGui::InputInt(varName.c_str(), &value)) {
                    var.value = value;
                }
                break;
            }
            case VariableType::String:
            {
                std::string value = std::get<std::string>(var.value);
                char buf[256];
                std::snprintf(buf, sizeof(buf), "%s", value.c_str());
                if (ImGui::InputText(varName.c_str(), buf, IM_ARRAYSIZE(buf))) {
                    var.value = std::string(buf);
                }
                break;
            }
            }
        }

        ImGui::Separator();
        ImGui::Text("Add New Variable:");
        ImGui::InputText("Name", newVarName, IM_ARRAYSIZE(newVarName));

        const char* types[] = {"Bool","Int","String"};
        ImGui::Combo("Type", &newVarTypeIndex, types, 3);

        if (newVarTypeIndex == 0) {
            ImGui::Checkbox("Initial Bool Value", &newVarBoolValue);
        } else if (newVarTypeIndex == 1) {
            ImGui::InputInt("Initial Int Value", &newVarIntValue);
        } else {
            ImGui::InputText("Initial String Value", newVarStringValue, IM_ARRAYSIZE(newVarStringValue));
        }

        if (ImGui::Button("Add Variable")) {
            std::string nameStr(newVarName);
            if (!nameStr.empty()) {
                // Build the variant and type
                VariableType vtype;
                std::variant<bool, int, std::string> initVal;

                if (newVarTypeIndex == 0) {
                    vtype = VariableType::Bool;
                    initVal = newVarBoolValue;
                }
                else if (newVarTypeIndex == 1) {
                    vtype = VariableType::Int;
                    initVal = newVarIntValue;
                }
                else {
                    vtype = VariableType::String;
                    initVal = std::string(newVarStringValue);
                }

                // Insert into the graph via the manager
                vsEditor->GetMainManager()->AddVariable(nameStr, vtype, initVal);
            }
        }

        ImGui::Separator();
        ImGui::Text("Functions:");
        ImGui::Separator();

        // List existing function graphs
        auto functions = vsEditor->GetFunctionNames();
        for (auto& fn : functions) {
            bool isSelected = (fn == vsEditor->GetCurrentGraphName());
            if (ImGui::Selectable(fn.c_str(), isSelected)) {
                vsEditor->SwitchToGraph(fn);
            }
        }

        ImGui::Separator();
        ImGui::Text("Add New Function:");
        static char newFunctionNameBuffer[128] = "";
        ImGui::InputText("Function Name", newFunctionNameBuffer, IM_ARRAYSIZE(newFunctionNameBuffer));
        if (ImGui::Button("Create Function")) {
            std::string fn(newFunctionNameBuffer);
            if (!fn.empty()) {
                vsEditor->NewFunction(fn);
                vsEditor->SwitchToGraph(fn);
            }
        }
    }

    void NodeEditorLeftPanelUI::RenderStateMachinePanel(StateMachineEditor* smEditor) {
        // For a state machine, you might list states or booleans here.
        // For now, just show a placeholder.

        ImGui::Text("State Machine Properties");
        ImGui::Separator();
        ImGui::Text("Here you can list states or transitions, etc.");
    }

}
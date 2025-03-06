#pragma once
#include <string>

namespace TulparEngine::EngineEditor {
    class BaseNodeEditor;
    class VisualScriptEditor;
    class StateMachineEditor;

    class NodeEditorLeftPanelUI {
    public:
        static NodeEditorLeftPanelUI& GetInstance();

        // Render the left panel
        void Render(BaseNodeEditor* editor);

    private:
        NodeEditorLeftPanelUI() = default;
        ~NodeEditorLeftPanelUI() = default;

        NodeEditorLeftPanelUI(const NodeEditorLeftPanelUI&) = delete;
        NodeEditorLeftPanelUI& operator=(const NodeEditorLeftPanelUI&) = delete;

        // Internal helpers
        void RenderVisualScriptPanel(VisualScriptEditor* vsEditor);
        void RenderStateMachinePanel(StateMachineEditor* smEditor);

        // Temporary state for adding a new variable
        char newVarName[128] = "";
        int newVarTypeIndex = 0; // 0: Bool, 1: Int, 2: String
        int newVarIntValue = 0;
        bool newVarBoolValue = false;
        char newVarStringValue[128] = "";
    };
}
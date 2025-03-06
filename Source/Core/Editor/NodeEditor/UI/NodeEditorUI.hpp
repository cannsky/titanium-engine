#pragma once
#include <string>

namespace TulparEngine::EngineEditor {

    class BaseNodeEditor;

    // NodeEditorUI is responsible for rendering the UI panels, node editor space, etc.
    class NodeEditorUI {
    public:
        static NodeEditorUI& GetInstance();

        void RenderGui(BaseNodeEditor* currentEditor);
        // You might have additional methods for handling dialogs, menus, etc.

    private:
        NodeEditorUI();
        ~NodeEditorUI();

        // Helper functions
        void ShowMainMenuBar();
        void RenderVisualScriptUI(BaseNodeEditor* vsEditor);
        void RenderStateMachineUI(BaseNodeEditor* smEditor);
    };

}
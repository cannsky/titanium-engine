#pragma once

#include "EngineUI.hpp"

namespace TulparEngine::EngineEditor {
    class EditorTopBarUI : public EngineUI {
    public:
        EditorTopBarUI();
        ~EditorTopBarUI() override;

        void renderGui() override;

        // Track the visibility state of the editor window
        bool showEditorWindow = false; 
    private:
    };
}
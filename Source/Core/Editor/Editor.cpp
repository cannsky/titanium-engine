#include "Editor.hpp"

namespace TulparEngine::EngineEditor {
    Editor::Editor() {
        // Private constructor for singleton
    }

    Editor::~Editor() {
        // Destructor
    }

    void Editor::Init() {
        EditorUIManager::GetInstance().Init();
        EditorWindowManager::GetInstance().Init();
    }

    void Editor::run() {
        // EditorUIManager::GetInstance().run();
    }

    void Editor::cleanup() {
        // Cleanup all systems
    }
}
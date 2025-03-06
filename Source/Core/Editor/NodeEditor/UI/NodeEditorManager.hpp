#pragma once
#include <string>
#include <vector>
#include <memory>
#include "BaseNodeEditor.hpp"

namespace TulparEngine::EngineEditor {

    // NodeEditorManager manages multiple editors and handles loading/saving them.
    class NodeEditorManager {
    public:
        static NodeEditorManager& GetInstance();

        BaseNodeEditor* CreateNewVisualScriptEditor(const std::string& name);
        BaseNodeEditor* CreateNewStateMachineEditor(const std::string& name);

        BaseNodeEditor* LoadEditor(const std::string& filePath, bool isVisualScript);
        bool SaveCurrentEditor(const std::string& filePath);

        BaseNodeEditor* GetCurrentEditor() { return currentEditor; }
        void CloseCurrentEditor();

    private:
        NodeEditorManager() = default;
        ~NodeEditorManager() = default;

        NodeEditorManager(const NodeEditorManager&) = delete;
        NodeEditorManager& operator=(const NodeEditorManager&) = delete;

        std::vector<std::unique_ptr<BaseNodeEditor>> editors;
        BaseNodeEditor* currentEditor = nullptr;
    };
}
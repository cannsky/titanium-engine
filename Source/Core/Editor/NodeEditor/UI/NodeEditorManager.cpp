#include "NodeEditorManager.hpp"
#include "VisualScriptEditor.hpp"
#include "StateMachineEditor.hpp"
#include <stdexcept>

namespace TulparEngine::EngineEditor {

    NodeEditorManager& NodeEditorManager::GetInstance() {
        static NodeEditorManager instance;
        return instance;
    }

    BaseNodeEditor* NodeEditorManager::CreateNewVisualScriptEditor(const std::string& name) {
        auto editor = std::make_unique<VisualScriptEditor>(name);
        BaseNodeEditor* ptr = editor.get();
        editors.push_back(std::move(editor));
        currentEditor = ptr;
        return ptr;
    }

    BaseNodeEditor* NodeEditorManager::CreateNewStateMachineEditor(const std::string& name) {
        auto editor = std::make_unique<StateMachineEditor>(name);
        BaseNodeEditor* ptr = editor.get();
        editors.push_back(std::move(editor));
        currentEditor = ptr;
        return ptr;
    }

    BaseNodeEditor* NodeEditorManager::LoadEditor(const std::string& filePath, bool isVisualScript) {
        std::unique_ptr<BaseNodeEditor> editor;
        if (isVisualScript) {
            editor = std::make_unique<VisualScriptEditor>("LoadedScript");
        } else {
            editor = std::make_unique<StateMachineEditor>("LoadedStateMachine");
        }

        if (editor->LoadFromFile(filePath)) {
            BaseNodeEditor* ptr = editor.get();
            editors.push_back(std::move(editor));
            currentEditor = ptr;
            return ptr;
        } else {
            throw std::runtime_error("Failed to load the editor from file: " + filePath);
        }
    }

    bool NodeEditorManager::SaveCurrentEditor(const std::string& filePath) {
        if (!currentEditor) return false;
        return currentEditor->SaveToFile(filePath);
    }

    void NodeEditorManager::CloseCurrentEditor() {
        if (!currentEditor) return;
        editors.erase(std::remove_if(editors.begin(), editors.end(),
                                     [&](auto& e){return e.get() == currentEditor;}), editors.end());
        currentEditor = nullptr;
    }

}
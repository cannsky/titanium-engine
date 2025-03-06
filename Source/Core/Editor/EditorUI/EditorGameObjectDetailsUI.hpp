#pragma once

#include <string>
#include <vector>
#include <memory>
#include "imgui.h"
#include "EngineUI.hpp"
#include "EngineGameObject.hpp"
#include "EngineComponent.hpp"
#include "EditorAssistantChatUI.hpp"

namespace TulparEngine::EngineEditor {
    class EditorGameObjectDetailsUI : public EngineUI {
    public:
        // Singleton access method for the EditorGameObjectDetailsUI
        static EditorGameObjectDetailsUI& GetInstance() {
            static EditorGameObjectDetailsUI instance;
            return instance;
        }

        // Constructor and destructor
        EditorGameObjectDetailsUI();
        ~EditorGameObjectDetailsUI() override;

        // Deleting copy constructor and assignment operator to enforce singleton
        EditorGameObjectDetailsUI(const EditorGameObjectDetailsUI&) = delete;
        void operator=(const EditorGameObjectDetailsUI&) = delete;

        // Render GUI method
        void renderGui() override;

        // Stores the currently selected entity ID and its components
        EntityID selectedEntityID = 0;
        std::vector<std::shared_ptr<EngineComponent>> components;

        // Store addable components
        std::vector<ComponentType> addableComponents;

        float panelHeight;

    private:
        void renderGameObjectDetails();
    };
}
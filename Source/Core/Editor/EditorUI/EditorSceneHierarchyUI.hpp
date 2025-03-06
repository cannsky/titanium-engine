#pragma once

#include "imgui.h"

#include "EngineUI.hpp"
#include "EngineGameObject.hpp"
#include "EngineEntity.hpp"
#include "EngineGraphicsManager.hpp"
#include "EngineSceneManager.hpp"

namespace TulparEngine::EngineEditor {
    class EditorSceneHierarchyUI : public EngineUI {
    public:
        // Singleton access method for EngineFrameManager
        static EditorSceneHierarchyUI& GetInstance() {
            static EditorSceneHierarchyUI instance;
            return instance;
        }

        EditorSceneHierarchyUI();
        ~EditorSceneHierarchyUI() override;

        // Deleting copy constructor and assignment operator to enforce singleton
        EditorSceneHierarchyUI(const EditorSceneHierarchyUI&) = delete;
        void operator=(const EditorSceneHierarchyUI&) = delete;

        // Stores the currently selected entity ID
        EntityID selectedEntityID = 0;

        void renderGui() override;
    private:
    };
}
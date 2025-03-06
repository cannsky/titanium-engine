#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

#include "EngineWindowManager.hpp"

using namespace TulparEngine;

namespace TulparEngine::EngineEditor {
    class EditorWindowManager {
    public:
        // Singleton access method for EditorWindowManager
        static EditorWindowManager& GetInstance() {
            static EditorWindowManager instance;
            return instance;
        }

        void Init();

        void start();

        void run();

        void cleanup();

        // Deleting copy constructor and assignment operator to enforce singleton
        EditorWindowManager(const EditorWindowManager&) = delete;
        void operator=(const EditorWindowManager&) = delete;
    private:
        EditorWindowManager();  // Private constructor for singleton
        ~EditorWindowManager();
    };
}
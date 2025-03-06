#pragma once

#include "EditorUIManager.hpp"
#include "EditorWindowManager.hpp"

namespace TulparEngine::EngineEditor {
    class Editor {
    public:
        // Singleton access method for the Editor
        static Editor& GetInstance() {
            static Editor instance;
            return instance;
        }

        // Initialize all systems
        void Init();

        // Run the main engine loop
        void run();

        // Cleanup all systems
        void cleanup();

        // Deleting copy constructor and assignment operator to enforce singleton
        Editor(const Editor&) = delete;
        void operator=(const Editor&) = delete;

        // Boolean for debug mode
        bool isDebugModeEnabled = true;
    private:
        Editor();  // Private constructor for singleton
        ~Editor();
    };
}
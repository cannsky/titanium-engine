#pragma once

namespace TulparEngine {
    class EngineVisualDebugManager {
    public:
        // Singleton access method for EngineVisualDebugManager
        static EngineVisualDebugManager& GetInstance() {
            static EngineVisualDebugManager instance;
            return instance;
        }

        void Init();
        void start();
        void run();
        void cleanup();

        // Deleting copy constructor and assignment operator to enforce singleton
        EngineVisualDebugManager(const EngineVisualDebugManager&) = delete;
        void operator=(const EngineVisualDebugManager&) = delete;
    private:
        EngineVisualDebugManager();  // Private constructor for singleton
        ~EngineVisualDebugManager();

        void draw();
    };
}
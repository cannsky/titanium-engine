#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <fstream>

namespace TulparEngine {
    class EngineFileManager {
    public:
        // Singleton access method for EngineFileManager
        static EngineFileManager& GetInstance() {
            static EngineFileManager instance;
            return instance;
        }

        // Initialize all systems
        void Init();
        // Run the main engine loop
        void run();
        // Cleanup all systems
        void cleanup();

        static std::vector<char> readFile(const std::string& filePath);

        // Deleting copy constructor and assignment operator to enforce singleton
        EngineFileManager(const EngineFileManager&) = delete;
        void operator=(const EngineFileManager&) = delete;

    private:
        EngineFileManager();  // Private constructor for singleton
        ~EngineFileManager();
    };
}
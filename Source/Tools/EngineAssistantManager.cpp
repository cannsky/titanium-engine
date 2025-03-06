#include "EngineAssistantManager.hpp"

namespace TulparEngine::Tools {
    EngineAssistantManager::EngineAssistantManager() {

    }

    EngineAssistantManager::~EngineAssistantManager() {
        // Cleanup all systems
        Cleanup();
    }

    void EngineAssistantManager::Init() {
        //StartAssistantServer();
    }

    void EngineAssistantManager::Start() {

    }

    void EngineAssistantManager::Run() {

    }

    void EngineAssistantManager::Cleanup() {
        //StopAssistantServer();
    }

    void EngineAssistantManager::StartAssistantServer() {
        EngineDebugManager::log("Starting Assistant Server", DebugCategory::Assistant);
        npmProcessPid = fork(); // Create a new process
        if (npmProcessPid == 0) {
            // Child process: Run the npm command
            std::string command = std::string("cd ") + ENGINE_DIR + "External/aiassistant && npm run dev";
            std::system(command.c_str()); // Execute the command
            std::exit(0); // Ensure the child process exits after running the command
        } else if (npmProcessPid > 0) {
            EngineDebugManager::log("npm run dev started with PID: " + std::to_string(npmProcessPid), DebugCategory::Assistant);
        } else {
            // Fork failed
            EngineDebugManager::log("Failed to start npm run dev.", DebugCategory::Assistant);
        }
    }

    void EngineAssistantManager::StopAssistantServer() {
        EngineDebugManager::log("Stopping Assistant Server", DebugCategory::Assistant);
        if (npmProcessPid > 0) {
                // Kill the npm process using the PID
                int result = kill(npmProcessPid, SIGKILL);
                if (result == 0) {
                    EngineDebugManager::log("npm run dev terminated successfully.", DebugCategory::Assistant);
                } else {
                    EngineDebugManager::log("Failed to terminate npm run dev.", DebugCategory::Assistant);
                }
            } else {
                EngineDebugManager::log("npm run dev process not found.", DebugCategory::Assistant);
            }
        }
}
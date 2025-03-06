#include "EngineTools.hpp"

namespace TulparEngine::Tools {
    
    // Private constructor
    EngineTools::EngineTools() {}
    
    // Destructor
    EngineTools::~EngineTools() {}
    
    // Initialize all systems
    void EngineTools::Init() {
        if (isAssistantEnabled) EngineAssistantManager::GetInstance().Init();
        #ifdef QT_ENABLED
        if (isBrowserEnabled) EngineBrowserManager::GetInstance().Init();
        #endif
    }
    
    // Start all systems
    void EngineTools::Start() {
        
    }
    
    // Run the main engine loop
    void EngineTools::Run() {
        if (isAssistantEnabled) EngineAssistantManager::GetInstance().Run();
        if (isRequestEnabled) EngineRequestManager::GetInstance().Run();
    }
    
    // Cleanup all systems
    void EngineTools::Cleanup() {
        
    }
}
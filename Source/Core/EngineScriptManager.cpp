#include "EngineScriptManager.hpp"

namespace TulparEngine {
    EngineScriptManager::EngineScriptManager() {}

    EngineScriptManager::~EngineScriptManager() { cleanupScripts(); }

    void EngineScriptManager::Init() {}

    void EngineScriptManager::run(float deltaTime) { runScripts(deltaTime); }

    void EngineScriptManager::cleanup() { cleanupScripts(); }

    void EngineScriptManager::initScripts() {
        for (auto& script : scripts) { script->Init(); }
    }

    void EngineScriptManager::runScripts(float deltaTime) {
        for (auto& script : scripts) { script->run(deltaTime); }
    }

    void EngineScriptManager::cleanupScripts() {
        for (auto& script : scripts) { script->cleanup(); }
        scripts.clear();
    }

    void EngineScriptManager::addScript(std::shared_ptr<EngineScript> script) {
        scripts.push_back(script);
    }

    void EngineScriptManager::removeScript(std::shared_ptr<EngineScript> script) {
        auto it = std::find(scripts.begin(), scripts.end(), script);
        if (it != scripts.end()) {
            scripts.erase(it);
        }
    }
}
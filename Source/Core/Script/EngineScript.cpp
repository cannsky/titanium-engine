#include "EngineScriptManager.hpp"

namespace TulparEngine {
    // Add the script to the script manager when it is created
    EngineScript::EngineScript() { EngineScriptManager::GetInstance().addScript(shared_from_this()); }

    // Remove the script from the script manager when it is destroyed
    EngineScript::~EngineScript() { EngineScriptManager::GetInstance().removeScript(shared_from_this()); }
}
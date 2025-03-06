#pragma once

#include "EngineAISystemManager.hpp"
#include <memory>

#define REGISTER_SYSTEM(SystemClass) \
namespace { \
    bool registered_##SystemClass = []() { \
        TulparEngine::EngineAISystemManager::GetInstance().AddSystem( \
            std::make_unique<SystemClass>() \
        ); \
        return true; \
    }(); \
}
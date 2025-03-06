#pragma once

#include "EngineEntityManager.hpp"
#include <memory>

#define REGISTER_SYSTEM(SystemClass) \
namespace { \
    bool registered_##SystemClass = []() { \
        TulparEngine::EngineEntityManager::GetInstance().AddSystem( \
            std::make_unique<SystemClass>() \
        ); \
        return true; \
    }(); \
}
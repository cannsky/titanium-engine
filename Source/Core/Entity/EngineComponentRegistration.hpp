#pragma once

#include "EngineEntityManager.hpp"
#include <memory>

#define REGISTER_COMPONENT(ComponentClass, Name) \
namespace { \
    bool registered_##ComponentClass = []() { \
        TulparEngine::EngineEntityManager::GetInstance().RegisterComponentType( \
            typeid(ComponentClass), \
            Name, \
            []() { return std::make_shared<ComponentClass>(); } \
        ); \
        return true; \
    }(); \
}
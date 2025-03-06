#pragma once

#include <typeindex>

namespace TulparEngine {
    using ComponentType = std::type_index;

    class EngineComponent {
    public:
        virtual ~EngineComponent() = default;
        virtual ComponentType GetType() const = 0;
    };
}
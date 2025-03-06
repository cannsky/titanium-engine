#pragma once

#include <vector>
#include "EngineComponent.hpp"
#include "EngineArchetype.hpp"

namespace TulparEngine {
    class EngineSystem {
    public:
        virtual ~EngineSystem() = default;

        virtual const std::vector<ComponentType>& GetComponentTypes() const = 0;

        virtual void UpdateBatch(EngineArchetype& archetype, size_t startIdx, size_t endIdx) = 0;
    };
}
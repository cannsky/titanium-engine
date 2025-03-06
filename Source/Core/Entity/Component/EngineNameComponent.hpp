#pragma once

#include <string>
#include "EngineComponent.hpp"

namespace TulparEngine {
    class EngineNameComponent : public EngineComponent {
    public:
        EngineNameComponent();
        virtual ComponentType GetType() const override;

        std::string name{};
    };
}
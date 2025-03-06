#pragma once

#include <vector>
#include <functional>

namespace TulparEngine {
    using StateID = int;

    struct EngineTransition {
        StateID targetStateID;
        std::function<bool()> condition;
    };
}
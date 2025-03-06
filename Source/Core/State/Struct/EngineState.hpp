#pragma once

#include "EngineTransition.hpp"

#include <vector>
#include <functional>
#include <absl/container/flat_hash_map.h>

namespace TulparEngine {
    using StateID = int;

    struct EngineState {
        StateID id;
        absl::flat_hash_map<StateID, EngineTransition> transitions;
    };
}
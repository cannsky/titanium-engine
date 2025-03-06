#pragma once

#include <typeindex>
#include <bitset>

namespace TulparEngine {
    using ComponentType = std::type_index;

    const size_t MAX_COMPONENT_TYPES = 64;
    using ComponentSignature = std::bitset<MAX_COMPONENT_TYPES>;
}
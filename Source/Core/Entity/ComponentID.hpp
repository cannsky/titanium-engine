#pragma once

#include <cstddef>

namespace TulparEngine {
    template<typename T>
    struct ComponentID {
        static size_t Get() {
            static const size_t id = nextID++;
            return id;
        }
    private:
        static size_t nextID;
    };

    template<typename T>
    size_t ComponentID<T>::nextID = 0;
}
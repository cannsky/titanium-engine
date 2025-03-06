#pragma once

#include "EngineEventManager.hpp"
#include "EngineEntity.hpp"

namespace TulparEngine {
    class EngineOnTriggerEvent : public Event {
    public:
        EngineOnTriggerEvent(EntityID entityA, EntityID entityB) : entityA(entityA), entityB(entityB) {}
        EntityID entityA;
        EntityID entityB;
    };
}
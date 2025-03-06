#pragma once

#include "EngineEntity.hpp"

namespace TulparEngine::Examples::SimpleShooter {
    // PlayerEntity derived from EngineEntity
    class PlayerEntity : public EngineEntity {
    public:
        PlayerEntity(EntityID id) : EngineEntity(id) {}
    };
}
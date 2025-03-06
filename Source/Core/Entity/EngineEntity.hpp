#pragma once

namespace TulparEngine {
    using EntityID = unsigned int;

    class EngineEntity {
    public:
        EngineEntity(EntityID id) : entityID(id) {}
        virtual ~EngineEntity() {}

        EntityID GetID() const { return entityID; }

    private:
        EntityID entityID;
    };
}
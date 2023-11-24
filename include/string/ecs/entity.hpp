#pragma once

#include <array>
#include <bitset>
#include <cassert>
#include <cstdint>
#include <queue>

namespace String {

using Entity = std::uint32_t;
static constexpr uint32_t MAX_ENTITIES = 5000;

using ComponentType = std::uint8_t;
static constexpr uint32_t MAX_COMPONENTS = 32;

using Signature = std::bitset<MAX_COMPONENTS>;

class EntityManager {
public:
    EntityManager(const uint32_t& max_entities = 5000) : max_entities_(max_entities) {
        for (Entity entity = 0; entity < max_entities; ++entity) {
            active_entity_ids_.push(entity);
        }
    }

    Entity create_entity() {
        assert(living_entity_count_ < max_entities_ && "Too many entities in existence.");

        Entity new_entity = active_entity_ids_.front();
        active_entity_ids_.pop();
        ++living_entity_count_;

        return new_entity;
    }

    void destroy_entity(Entity entity) {
        assert(entity < max_entities_ && "Entity out of range.");

        signatures_[entity].reset();
        active_entity_ids_.push(entity);
        --living_entity_count_;
    }

    void set_signature(Entity entity, Signature signature) {
        assert(entity < max_entities_ && "Entity out of range.");

        signatures_[entity] = signature;
    }

    Signature get_signature(Entity entity) {
        assert(entity < max_entities_ && "Entity out of range.");

        return signatures_[entity];
    }

private:
    std::queue<Entity> active_entity_ids_{};
    std::array<Signature, MAX_ENTITIES> signatures_{};
    uint32_t living_entity_count_{0};
    uint32_t max_entities_;
};
}  // namespace String

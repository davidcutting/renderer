#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <string/ecs/entity.hpp>
#include <unordered_map>

namespace String {

class IComponentArray {
public:
    virtual ~IComponentArray() = default;
    virtual void entity_destroyed(Entity entity) = 0;
};

template <typename T>
class ComponentArray : public IComponentArray {
public:
    void insert_data(Entity entity, T component) {
        assert(entity_to_index_map_.find(entity) == entity_to_index_map_.end() &&
               "Component added to same entity more than once.");

        // Put new entry at end
        size_t new_index = size_;
        entity_to_index_map_[entity] = new_index;
        index_to_entity_map[new_index] = entity;
        components_[new_index] = component;
        ++size_;
    }

    void remove_data(Entity entity) {
        assert(entity_to_index_map_.find(entity) != entity_to_index_map_.end() && "Removing non-existent component.");

        // Copy element at end into deleted element's place to maintain density
        size_t removed_entity_index = entity_to_index_map_[entity];
        size_t last_element_index = size_ - 1;
        components_[removed_entity_index] = components_[last_element_index];

        // Update map to point to moved spot
        Entity last_element_entity = index_to_entity_map[last_element_index];
        entity_to_index_map_[last_element_entity] = removed_entity_index;
        index_to_entity_map[removed_entity_index] = last_element_entity;

        entity_to_index_map_.erase(entity);
        index_to_entity_map.erase(last_element_index);

        --size_;
    }

    T& get_data(Entity entity) {
        assert(entity_to_index_map_.find(entity) != entity_to_index_map_.end() && "Retrieving non-existent component.");

        return components_[entity_to_index_map_[entity]];
    }

    void entity_destroyed(Entity entity) override {
        if (entity_to_index_map_.find(entity) != entity_to_index_map_.end()) {
            remove_data(entity);
        }
    }

private:
    std::array<T, 5000> components_{};
    std::unordered_map<Entity, size_t> entity_to_index_map_;
    std::unordered_map<size_t, Entity> index_to_entity_map{};
    size_t size_{};
};

}  // namespace String
#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <string/core/entity.hpp>
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

class ComponentManager {
public:
    template <typename T>
    void register_component() {
        const char* typeName = typeid(T).name();

        assert(component_types_.find(typeName) == component_types_.end() &&
               "Registering component type more than once.");

        component_types_.insert({typeName, next_component_type_});
        component_arrays_.insert({typeName, std::make_shared<ComponentArray<T>>()});

        ++next_component_type_;
    }

    template <typename T>
    ComponentType get_component_type() {
        const char* typeName = typeid(T).name();

        assert(component_types_.find(typeName) != component_types_.end() && "Component not registered before use.");

        return component_types_[typeName];
    }

    template <typename T>
    void add_component(Entity entity, T component) {
        get_component_array<T>()->InsertData(entity, component);
    }

    template <typename T>
    void remove_component(Entity entity) {
        get_component_array<T>()->RemoveData(entity);
    }

    template <typename T>
    T& get_component(Entity entity) {
        return get_component_array<T>()->GetData(entity);
    }

    void entity_destroyed(Entity entity) {
        for (const auto& pair : component_arrays_) {
            const auto& component = pair.second;

            component->entity_destroyed(entity);
        }
    }

private:
    std::unordered_map<const char*, ComponentType> component_types_{};
    std::unordered_map<const char*, std::shared_ptr<IComponentArray>> component_arrays_{};
    ComponentType next_component_type_{};

    template <typename T>
    std::shared_ptr<ComponentArray<T>> get_component_array() {
        const char* typeName = typeid(T).name();

        assert(component_types_.find(typeName) != component_types_.end() && "Component not registered before use.");

        return std::static_pointer_cast<ComponentArray<T>>(component_arrays_[typeName]);
    }
};

}  // namespace String
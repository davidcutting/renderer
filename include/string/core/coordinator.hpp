#pragma once

#include <string/core/component.hpp>
#include <string/core/entity.hpp>
#include <string/core/system.hpp>
#include <string/core/event.hpp>

#include <bits/stdc++.h>

namespace String {

class Coordinator {
public:
    void init() {
        component_manager_ = std::make_unique<ComponentManager>();
        entity_manager_ = std::make_unique<EntityManager>();
        event_manager_ = std::make_unique<EventManager>();
        system_manager_ = std::make_unique<SystemManager>();
    }

    // Entity methods
    Entity create_entity() { return entity_manager_->create_entity(); }

    void destroy_entity(Entity entity) {
        entity_manager_->destroy_entity(entity);

        component_manager_->entity_destroyed(entity);

        system_manager_->entity_destroyed(entity);
    }

    // Component methods
    template <typename T>
    void register_component() {
        component_manager_->register_component<T>();
    }

    template <typename T>
    void add_component(Entity entity, T component) {
        component_manager_->add_component<T>(entity, component);

        auto signature = entity_manager_->get_signature(entity);
        signature.set(component_manager_->get_component_type<T>(), true);
        entity_manager_->set_signature(entity, signature);

        system_manager_->entity_signature_changed(entity, signature);
    }

    template <typename T>
    void remove_component(Entity entity) {
        component_manager_->remove_component<T>(entity);

        auto signature = entity_manager_->get_signature(entity);
        signature.set(component_manager_->get_component_type<T>(), false);
        entity_manager_->set_signature(entity, signature);

        system_manager_->entity_signature_changed(entity, signature);
    }

    template <typename T>
    T& get_component(Entity entity) {
        return component_manager_->get_component<T>(entity);
    }

    template <typename T>
    ComponentType get_component_type() {
        return component_manager_->get_component_type<T>();
    }

    // System methods
    template <typename T>
    std::shared_ptr<T> register_system() {
        return system_manager_->register_system<T>();
    }

    template <typename T>
    void set_system_signature(Signature signature) {
        system_manager_->set_signature<T>(signature);
    }

    // Event methods
    void add_event_listener(EventId event_id, std::function<void(Event&)> const& listener) {
        event_manager_->add_listener(event_id, listener);
    }

    void send_event(Event& event) { event_manager_->send_event(event); }

    void send_event(EventId event_id) { event_manager_->send_event(event_id); }

private:
    std::unique_ptr<ComponentManager> component_manager_;
    std::unique_ptr<EntityManager> entity_manager_;
    std::unique_ptr<EventManager> event_manager_;
    std::unique_ptr<SystemManager> system_manager_;
};

}  // namespace String
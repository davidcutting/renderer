#pragma once

#include <bits/stdc++.h>

#include <string/core/entity.hpp>

namespace String {

class System {
public:
    std::set<Entity> entities;

    virtual ~System() = default;
    virtual auto update(float dt) -> void = 0;
};

class SystemManager {
public:
    template <typename T>
    void register_system(const std::shared_ptr<T>& system) {
        const char* type_name = typeid(T).name();

        assert(systems_.find(type_name) == systems_.end() && "Registering system more than once.");

        systems_.insert({type_name, system});
    }

    template <typename T>
    void set_signature(Signature signature) {
        const char* type_name = typeid(T).name();

        assert(systems_.find(type_name) != systems_.end() && "System used before registered.");

        signatures_.insert({type_name, signature});
    }

    void entity_destroyed(Entity entity) {
        for (const auto& pair : systems_) {
            const auto& system = pair.second;

            system->entities.erase(entity);
        }
    }

    void entity_signature_changed(Entity entity, Signature entity_signature) {
        for (const auto& pair : systems_) {
            const auto& type = pair.first;
            const auto& system = pair.second;
            const auto& system_signature = signatures_[type];

            if ((entity_signature & system_signature) == system_signature) {
                system->entities.insert(entity);
            } else {
                system->entities.erase(entity);
            }
        }
    }

private:
    std::unordered_map<const char*, Signature> signatures_{};
    std::unordered_map<const char*, std::shared_ptr<System>> systems_{};
};

}  // namespace String
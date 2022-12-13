#pragma once

#include <string/component/physics_components.hpp>
#include <string/core/coordinator.hpp>
#include <string/core/system.hpp>

namespace String {

class PhysicsSystem : public System {
public:
    void Init() {}

    void Update(float dt) {
        for (const auto& entity : entities) {
            auto& rigid_body = coordinator->get_component<RigidBody>(entity);
            auto& transform = coordinator->get_component<Transform>(entity);

            // Forces
            const auto& gravity = coordinator->get_component<Gravity>(entity);

            transform.position += rigid_body.velocity * dt;

            rigid_body.velocity += gravity.force * dt;
        }
    }

private:
    std::shared_ptr<Coordinator> coordinator;
};

}  // namespace String

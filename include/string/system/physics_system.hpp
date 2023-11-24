#pragma once

#include <string/component/physics_components.hpp>
#include <string/core/coordinator.hpp>
#include <string/core/system.hpp>

namespace String {

class PhysicsSystem : public System {
public:
    auto update(float dt) -> void override {
        auto startTime = std::chrono::high_resolution_clock::now();

        for (const auto& entity : entities) {
            auto& rigid_body = coordinator->get_component<RigidBody>(entity);
            auto& transform = coordinator->get_component<Transform>(entity);

            // Forces
            const auto& gravity = coordinator->get_component<Gravity>(entity);

            transform.position += rigid_body.velocity * dt;

            rigid_body.velocity += gravity.force * dt;
        }

        auto stopTime = std::chrono::high_resolution_clock::now();
        auto render_time = std::chrono::duration<float, std::chrono::seconds::period>(stopTime - startTime).count();
        frame_rate = dt / render_time;
    }

private:
    float frame_rate = 0.0f;
    
    std::shared_ptr<Coordinator> coordinator;
};

}  // namespace String

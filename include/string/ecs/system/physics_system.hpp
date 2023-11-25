#pragma once

#include <memory>
#include <string/ecs/component/physics_components.hpp>
#include <string/ecs/coordinator.hpp>
#include <string/ecs/system.hpp>

namespace String {

class PhysicsSystem : public System {
public:
    explicit PhysicsSystem(const std::shared_ptr<Coordinator>& coordinator) : coordinator(coordinator) {}
    
    auto update(float dt) -> void override {
        auto startTime = std::chrono::high_resolution_clock::now();

        for (const auto& entity : entities) {
            auto& rigid_body = coordinator->get_component<RigidBody>(entity);
            auto& transform = coordinator->get_component<Transform>(entity);

            // Forces
            // const auto& gravity = coordinator->get_component<Gravity>(entity);

            transform.position += rigid_body.velocity * dt;

            // no gravity :)
            // rigid_body.velocity += gravity.force * dt;
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

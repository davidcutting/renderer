#pragma once

#include <string/core/coordinator.hpp>
#include <string/core/system.hpp>
#include <string/component/physics_components.hpp>

namespace String {

class PhysicsSystem : public System {
public:
    void Init() {}

    void Update(float dt) {
        for (auto const& entity : mEntities) {
            auto& rigidBody = coordinator->GetComponent<RigidBody>(entity);
            auto& transform = coordinator->GetComponent<Transform>(entity);

            // Forces
            auto const& gravity = coordinator->GetComponent<Gravity>(entity);

            transform.position += rigidBody.velocity * dt;

            rigidBody.velocity += gravity.force * dt;
        }
    }

private:
    std::shared_ptr<Coordinator> coordinator;
};

}  // namespace String

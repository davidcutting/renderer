#include <cmath>
#include <memory>
#include <string/application.hpp>
#include <vector>
#include "string/ecs/entity.hpp"
#include "string/ecs/component/physics_components.hpp"
#include "string/ecs/component/render_components.hpp"
#include "string/ecs/system/physics_system.hpp"
#include "string/ecs/system/render_system.hpp"
#include "string/ecs/system/window_system.hpp"

namespace String {

Application* Application::instance = nullptr;

Application::Application(const ApplicationSpecification& specification) noexcept : specification(specification) {
    instance = this;

    coordinator = std::make_shared<Coordinator>();
    coordinator->init();
    coordinator->register_component<Mesh>();
    coordinator->register_component<Transform>();
    coordinator->register_component<RigidBody>();

    window_system = std::make_shared<WindowSystem>();
    coordinator->register_system(window_system);

    rendering_system = std::make_shared<RenderSystem>(coordinator, window_system->vulkan_window);
    coordinator->register_system(rendering_system);

    Signature render_signature;
    render_signature.set(coordinator->get_component_type<Mesh>());
    render_signature.set(coordinator->get_component_type<Transform>());
    coordinator->set_system_signature<RenderSystem>(render_signature);

    physics_system = std::make_shared<PhysicsSystem>(coordinator);
    coordinator->register_system(physics_system);

    Signature physics_signature;
    physics_signature.set(coordinator->get_component_type<RigidBody>());
    physics_signature.set(coordinator->get_component_type<Transform>());
    coordinator->set_system_signature<PhysicsSystem>(physics_signature);

    // Create one entity, a triangle
    std::vector<Entity> entities(1);

    for (auto& entity : entities) {
        entity = coordinator->create_entity();

        coordinator->add_component(
            entity, RigidBody{.velocity = Vec3(0.0f, 0.0f, 0.0f), .acceleration = Vec3(0.0f, 0.0f, 0.0f)});

        coordinator->add_component(
            entity, Transform{.position = Vec3(0, 0, 0), .rotation = Vec3(0, 0, 0), .scale = Vec3(1, 1, 1)});

        std::vector<Vertex> vertices = {
            {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        };
        
        coordinator->add_component(entity, Mesh{vertices});
    }
}

Application::~Application() {}

auto Application::run() const noexcept -> void {
    float dt = 0.0f;

    while (window_system->is_active()) {
        auto startTime = std::chrono::high_resolution_clock::now();

        window_system->update(dt);
        physics_system->update(dt);
        rendering_system->update(dt);

        auto stopTime = std::chrono::high_resolution_clock::now();
        dt = std::chrono::duration<float, std::chrono::seconds::period>(stopTime - startTime).count();
    }
}

}  // namespace String
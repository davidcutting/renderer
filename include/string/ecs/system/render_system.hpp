#pragma once

#include <memory>
#include <string/ecs/system.hpp>
#include <string/ecs/coordinator.hpp>
#include <string/vulkan/vulkan_renderer.hpp>
#include "string/ecs/component/physics_components.hpp"
#include "string/ecs/component/render_components.hpp"
#include "string/vulkan/vulkan_window.hpp"

namespace String {

class RenderSystem : public System {
public:
    explicit RenderSystem(const std::shared_ptr<Coordinator>& coordinator, const std::shared_ptr<Vulkan::VulkanWindow>& window) : coordinator(coordinator), renderer_(window) {}

    auto update(float dt) -> void override {
        auto startTime = std::chrono::high_resolution_clock::now();

        /*
        for (const auto& entity : entities) {
            auto& mesh = coordinator->get_component<Mesh>(entity);
            auto& transform = coordinator->get_component<Transform>(entity);
        }*/

        renderer_.draw_frame();

        auto stopTime = std::chrono::high_resolution_clock::now();
        auto render_time = std::chrono::duration<float, std::chrono::seconds::period>(stopTime - startTime).count();
        frame_rate = dt / render_time;
    }

private:
    float frame_rate = 0.0f;

    std::shared_ptr<Coordinator> coordinator;

    Vulkan::VulkanRenderer renderer_;
};

}  // namespace String

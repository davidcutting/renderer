#pragma once

#include <memory>
#include <string/core/system.hpp>
#include <string/vulkan/vulkan_renderer.hpp>
#include "string/vulkan/vulkan_window.hpp"

namespace String {

class RenderSystem : public System {
public:
    explicit RenderSystem(const std::shared_ptr<Vulkan::VulkanWindow>& window) : renderer_(window) {}

    auto update(float dt) -> void override {
        auto startTime = std::chrono::high_resolution_clock::now();

        renderer_.draw_frame();

        auto stopTime = std::chrono::high_resolution_clock::now();
        auto render_time = std::chrono::duration<float, std::chrono::seconds::period>(stopTime - startTime).count();
        frame_rate = dt / render_time;
    }

private:
    float frame_rate = 0.0f;

    Vulkan::VulkanRenderer renderer_;
};

}  // namespace String

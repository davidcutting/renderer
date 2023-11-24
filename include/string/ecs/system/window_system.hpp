#pragma once

#include <memory>
#include <string/ecs/coordinator.hpp>
#include <string/ecs/system.hpp>
#include <string/vulkan/vulkan_window.hpp>
#include <string/window.hpp>

namespace String {

class WindowSystem : public System {
public:
    WindowSystem() {
        Window::Properties properties{
            .title = "Hello Triangle",
            .mode = String::View::Mode::WINDOWED,
            .resizable = true,
            .vsync = String::View::VSync::ON,
            .extent = {800, 600},
        };

        vulkan_window = std::make_shared<Vulkan::VulkanWindow>(properties);
    }

    auto update(float dt) -> void override {
        auto startTime = std::chrono::high_resolution_clock::now();

        vulkan_window->update();

        auto stopTime = std::chrono::high_resolution_clock::now();
        auto render_time = std::chrono::duration<float, std::chrono::seconds::period>(stopTime - startTime).count();
        frame_rate = dt / render_time;
    }

    bool is_active() { return vulkan_window->is_open(); }

    std::shared_ptr<Vulkan::VulkanWindow> vulkan_window;

private:
    float frame_rate = 0.0f;

    std::shared_ptr<Coordinator> coordinator;
};

}  // namespace String
#pragma once

#include <memory>
#include <string/core/coordinator.hpp>
#include <string/core/system.hpp>
#include <string/vulkan/vulkan_window.hpp>
#include <string/window.hpp>

namespace String {

class WindowSystem : public System {
public:
    void init() {
        Window::Properties properties{
            .title = "Hello Triangle",
            .mode = String::View::Mode::WINDOWED,
            .resizable = true,
            .vsync = String::View::VSync::ON,
            .extent = {800, 600},
        };

        vulkan_window = std::make_shared<Vulkan::VulkanWindow>(properties);
    }

    void update() {
        vulkan_window->update();
    }

    bool is_active()
    {
        return vulkan_window->is_open();
    }

    std::shared_ptr<Vulkan::VulkanWindow> vulkan_window;

private:
    std::shared_ptr<Coordinator> coordinator;
};

}  // namespace String
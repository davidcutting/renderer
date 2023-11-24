#include <memory>
#include <string/application.hpp>
#include "string/system/render_system.hpp"
#include "string/system/window_system.hpp"

namespace String {

Application* Application::instance = nullptr;

Application::Application(const ApplicationSpecification& specification) noexcept : specification(specification) {
    instance = this;

    vertices = {
        {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    };

    coordinator = std::make_shared<Coordinator>();
    coordinator->init();

    window_system = std::make_shared<WindowSystem>();
    rendering_system = std::make_shared<RenderSystem>(window_system->vulkan_window);

    coordinator->register_system(window_system);
    coordinator->register_system(rendering_system);   
}

Application::~Application() {}

auto Application::run() const noexcept -> void {
    float dt = 0.0f;

    while (window_system->is_active()) {
        auto startTime = std::chrono::high_resolution_clock::now();

        window_system->update(dt);
        rendering_system->update(dt);

        auto stopTime = std::chrono::high_resolution_clock::now();
        dt = std::chrono::duration<float, std::chrono::seconds::period>(stopTime - startTime).count();
    }
}

}  // namespace String
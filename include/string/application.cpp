#include <string/application.hpp>

namespace String {

Application::Application(const ApplicationSpecification& specification) noexcept : specification(specification) {
    instance = this;

    vertices = {
        {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    };

    coordinator->init();

    window_system = coordinator->register_system<WindowSystem>();
    window_system->init();

    rendering_system = coordinator->register_system<RenderSystem>();
    rendering_system->set_window(window_system->vulkan_window);
    rendering_system->set_scene(vertices);
    rendering_system->init();
}

Application::~Application() { rendering_system->shutdown(); }

auto Application::run() const noexcept -> void {
    float dt = 0.0f;

    while (window_system->is_active()) {
        auto startTime = std::chrono::high_resolution_clock::now();

        window_system->update();
        rendering_system->update(dt);

        auto stopTime = std::chrono::high_resolution_clock::now();
        dt = std::chrono::duration<float, std::chrono::seconds::period>(stopTime - startTime).count();
    }
}

}  // namespace String
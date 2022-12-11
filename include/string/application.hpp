// MIT License
//
// Copyright (c) 2022 David Cutting
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <string/core/coordinator.hpp>
#include <string/system/render_system.hpp>
#include <string/vulkan/vulkan_window.hpp>
#include <string/window.hpp>

namespace String {

class HelloTriangleApplication {
public:
    HelloTriangleApplication() { init(); };
    ~HelloTriangleApplication() { shutdown(); }

    void init() {
        Window::Properties properties{
            .title = "Hello Triangle",
            .mode = String::View::Mode::WINDOWED,
            .resizable = true,
            .vsync = String::View::VSync::ON,
            .extent = {800, 600},
        };
        vulkan_window = std::make_shared<Vulkan::VulkanWindow>(properties);

        coordinator.Init();

        render_system = coordinator.RegisterSystem<RenderSystem>();
        render_system->set_window(vulkan_window);
        render_system->init();
    }

    void run() {
        float dt = 0.0f;

        while (vulkan_window->is_open()) {
            auto startTime = std::chrono::high_resolution_clock::now();

            vulkan_window->update();
            render_system->update(dt);

            auto stopTime = std::chrono::high_resolution_clock::now();
            dt = std::chrono::duration<float, std::chrono::seconds::period>(stopTime - startTime).count();
        }
    }

    void shutdown() { render_system->shutdown(); }

private:
    std::shared_ptr<Vulkan::VulkanWindow> vulkan_window;

    Coordinator coordinator;
    std::shared_ptr<RenderSystem> render_system;
};

}  // namespace String
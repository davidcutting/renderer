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

#include <bits/stdc++.h>

#include <string/core/coordinator.hpp>
#include <string/system/render_system.hpp>
#include <string/system/window_system.hpp>

namespace String {

class HelloTriangleApplication {
public:
    HelloTriangleApplication() {
        coordinator = std::make_shared<Coordinator>();

        coordinator->init();

        window_system = coordinator->register_system<WindowSystem>();
        window_system->init();

        rendering_system = coordinator->register_system<RenderSystem>();
        rendering_system->set_window(window_system->vulkan_window);
        rendering_system->init();
    };

    ~HelloTriangleApplication() { rendering_system->shutdown(); }

    void run() {
        float dt = 0.0f;

        while (window_system->is_active()) {
            auto startTime = std::chrono::high_resolution_clock::now();

            window_system->update();
            rendering_system->update(dt);

            auto stopTime = std::chrono::high_resolution_clock::now();
            dt = std::chrono::duration<float, std::chrono::seconds::period>(stopTime - startTime).count();
        }
    }

private:
    std::shared_ptr<Coordinator> coordinator;
    std::shared_ptr<RenderSystem> rendering_system;
    std::shared_ptr<WindowSystem> window_system;
};

}  // namespace String
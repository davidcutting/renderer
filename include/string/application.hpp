// MIT License
//
// Copyright (c) 2020-2023 David Cutting
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
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

#include <string/component/render_components.hpp>
#include <string/core/coordinator.hpp>
#include <string/system/render_system.hpp>
#include <string/system/window_system.hpp>

// Forward Declaration of Entrypoint Function
int main(int argc, char** argv);

namespace String {

struct ApplicationCommandLineArgs {
    int argc = 0;
    char** argv = nullptr;

    const char* operator[](int index) const {
        STRING_ASSERT(index < argc);
        return argv[index];
    }
};

struct ApplicationSpecification {
    std::string name = "String Application";
    std::string working_directory;
    ApplicationCommandLineArgs command_line_args;
};

class Application {
public:
    Application(const ApplicationSpecification& specification) noexcept;
    ~Application();

private:
    ApplicationSpecification specification;

    std::shared_ptr<Coordinator> coordinator;
    std::shared_ptr<RenderSystem> rendering_system;
    std::shared_ptr<WindowSystem> window_system;

    std::vector<Vertex> vertices;

    /** @brief Note this is a Singleton class as it is tied to the entrypoint, and there can only be one entrypoint.*/
    static Application* instance;

    /** @brief This should only be called once by the entrypoint. */
    auto run() const noexcept -> void;

    /** @brief Allow the entrypoint to call run() */
    friend int ::main(int argc, char** argv);
};

// To be defined in CLIENT
inline Application* CreateApplication(ApplicationCommandLineArgs args);

}  // namespace String

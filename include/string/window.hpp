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

#include <cinttypes>
#include <string>

namespace String {

namespace View {

struct Extent {
    uint32_t width;
    uint32_t height;
};

enum class Mode { WINDOWED, FULLSCREEN, FULLSCREEN_BORDERLESS, HEADLESS };

enum class VSync { OFF, ON };
}  // namespace View

class Window {
public:
    struct Properties {
        std::string title = "unnamed window";
        View::Mode mode = View::Mode::WINDOWED;
        bool resizable = true;
        View::VSync vsync = View::VSync::ON;
        View::Extent extent = {1280, 720};
    };

protected:
    Window::Properties _window_properties;

public:
    Window(const Window::Properties& properties) noexcept;
    virtual ~Window() = default;

    virtual void update() const noexcept = 0;

    virtual bool is_open() const noexcept = 0;

    virtual void* get_native_window() const noexcept = 0;

    Window::Properties get_properties() const noexcept;
};

inline Window::Window(const Window::Properties& properties) noexcept : _window_properties(properties) {
    // Constructor
}

inline Window::Properties Window::get_properties() const noexcept { return _window_properties; }

}  // namespace String
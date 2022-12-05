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

#include <string/window.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace String
{
class LinuxWindow final : String::Window
{
private:
    GLFWwindow* window_handle_;
public:
    LinuxWindow(const Properties& properties) noexcept;
    ~LinuxWindow() noexcept;

    virtual void* get_native_window() const noexcept override final;
}; // class LinuxWindow

inline LinuxWindow::LinuxWindow(const Window::Properties& properties) noexcept
    : String::Window(properties)
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window_handle_ = glfwCreateWindow(
        _window_properties.extent.width,
        _window_properties.extent.height,
        _window_properties.title.c_str(),
        nullptr,
        nullptr);

    glfwSetWindowUserPointer(window_handle_, this);
}

inline LinuxWindow::~LinuxWindow() noexcept
{
    glfwDestroyWindow(window_handle_);
    glfwTerminate();
}

inline void* LinuxWindow::get_native_window() const noexcept
{
    return window_handle_;
}

}  // namespace String
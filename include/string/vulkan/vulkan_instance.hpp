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

#include <GLFW/glfw3.h>
#include <bits/stdc++.h>
#include <vulkan/vulkan.h>

#include <string/application.hpp>
#include <string/core/debug.hpp>

namespace String {

using Version = std::tuple<uint8_t, uint8_t, uint8_t>;

class Application {
public:
    struct Properties {
        std::string name;
        Version version;
    };
};  // class Application

namespace Vulkan {

/**
    @class Instance
    @brief This class is responsible for initializing the Vulkan library, managing Vulkan extensions and validation
   layers, setting up Vulkan debug messaging if desired.
*/
class VulkanInstance {
private:
    /** @brief The application's properties */
    Application::Properties application_properties_;

    /** @brief The Vulkan instance */
    VkInstance instance_handle_;

public:
    /**
        @brief Constructor that initializes the Vulkan library
        @param properties The application's properties
        @throws runtime_error if the Vulkan instance fails to be created
    */
    VulkanInstance(const Application::Properties properties);

    ~VulkanInstance();

    VkInstance get_handle() const noexcept;
};

/**
    Impl
*/

inline VulkanInstance::VulkanInstance(const Application::Properties properties) : application_properties_(properties) {
    VkApplicationInfo application_info{
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = nullptr,  // TODO: Point to extension information
        .pApplicationName = application_properties_.name.c_str(),
        .applicationVersion =
            VK_MAKE_VERSION(std::get<0>(application_properties_.version), std::get<1>(application_properties_.version),
                            std::get<2>(application_properties_.version)),
        .pEngineName = "String Engine",
        .engineVersion = VK_MAKE_VERSION(0, 0, 1),
        .apiVersion = VK_API_VERSION_1_3,
    };

    // Setup Vulkan extensions
    uint32_t extension_count = 0;

    // Fill extension_count
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

    std::vector<VkExtensionProperties> extensions(extension_count);
    std::vector<const char*> required_extensions;

    // Get extensions
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());

    // Get the extensions used by the GLFW windowing library
    // TODO: Make generic to any windowing library
    {
        uint32_t glfw_extension_count = 0;
        const char** glfw_extensions;
        glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

        for (uint32_t i = 0; i < glfw_extension_count; ++i) {
            required_extensions.emplace_back(glfw_extensions[i]);
        }
    }

    VkInstanceCreateInfo create_info{
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .pApplicationInfo = &application_info,
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = nullptr,
        .enabledExtensionCount = (uint32_t)required_extensions.size(),
        .ppEnabledExtensionNames = required_extensions.data(),
    };

    // TODO: Use custom allocator
    const auto result = vkCreateInstance(&create_info, nullptr, &instance_handle_);
    STRING_ASSERT(result == VK_SUCCESS);
}

inline VulkanInstance::~VulkanInstance() {
    // TODO: Use custom allocator
    if (instance_handle_ != VK_NULL_HANDLE) {
        vkDestroyInstance(instance_handle_, nullptr);
    }
    // TODO: Log
}

inline VkInstance VulkanInstance::get_handle() const noexcept { return instance_handle_; }

}  // namespace Vulkan
}  // namespace String
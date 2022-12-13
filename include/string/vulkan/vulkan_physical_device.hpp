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

#include <bits/stdc++.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <string/core/debug.hpp>
#include <string/vulkan/vulkan_instance.hpp>

namespace String {
namespace Vulkan {

class PhysicalDevice {
private:
    VulkanInstance& instance_handle_;

    VkPhysicalDevice physical_device_handle_{VK_NULL_HANDLE};

    // The features that this GPU supports
    VkPhysicalDeviceFeatures physical_device_features_{};

    // The GPU properties
    VkPhysicalDeviceProperties physical_device_properties_;

    // The GPU memory properties
    VkPhysicalDeviceMemoryProperties physical_device_memory_properties_;

    // The GPU queue family properties
    std::vector<VkQueueFamilyProperties> queue_family_properties_;

    // The features that will be requested to be enabled in the logical device
    VkPhysicalDeviceFeatures physical_device_requested_features_{};

public:
    PhysicalDevice(VulkanInstance& instance) noexcept;

private:
    std::vector<VkPhysicalDevice> get_available_physical_devices(const VulkanInstance& instance) noexcept;

public:
    bool is_device_suitable(const VkPhysicalDevice& device) const noexcept;

    VkPhysicalDevice get_best_physical_device(const VulkanInstance& instance) noexcept;

};  // class PhysicalDevice

inline PhysicalDevice::PhysicalDevice(VulkanInstance& instance) noexcept : instance_handle_(instance) {
    std::vector<VkPhysicalDevice> devices = get_available_physical_devices(instance);

    vkGetPhysicalDeviceFeatures(physical_device_handle_, &physical_device_features_);
    vkGetPhysicalDeviceProperties(physical_device_handle_, &physical_device_properties_);
    vkGetPhysicalDeviceMemoryProperties(physical_device_handle_, &physical_device_memory_properties_);

    uint32_t queue_family_properties_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device_handle_, &queue_family_properties_count, nullptr);
    queue_family_properties_ = std::vector<VkQueueFamilyProperties>(queue_family_properties_count);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device_handle_, &queue_family_properties_count,
                                             queue_family_properties_.data());
}

inline std::vector<VkPhysicalDevice> PhysicalDevice::get_available_physical_devices(
    const VulkanInstance& instance) noexcept {
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(instance.get_handle(), &device_count, nullptr);

    STRING_ASSERT(device_count > 0);

    std::vector<VkPhysicalDevice> devices(device_count);
    vkEnumeratePhysicalDevices(instance.get_handle(), &device_count, devices.data());

    STRING_ASSERT(devices.size() > 0);
    return devices;
}

inline bool PhysicalDevice::is_device_suitable(const VkPhysicalDevice& device) const noexcept {
    VkPhysicalDeviceProperties device_properties;
    VkPhysicalDeviceFeatures device_features;
    vkGetPhysicalDeviceProperties(device, &device_properties);
}

inline VkPhysicalDevice PhysicalDevice::get_best_physical_device(const VulkanInstance& instance) noexcept {
    VkPhysicalDevice best_physical_device{VK_NULL_HANDLE};
    std::vector<VkPhysicalDevice> devices = get_available_physical_devices(instance);

    for (const auto& device : devices) {
        if (is_device_suitable(device)) {
            best_physical_device = device;
            break;
        }
    }

    STRING_ASSERT(best_physical_device != VK_NULL_HANDLE);

    return best_physical_device;
}

}  // namespace Vulkan
}  // namespace String
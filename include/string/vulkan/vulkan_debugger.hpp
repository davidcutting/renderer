#pragma once

#include <bits/stdc++.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

inline VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                             const VkAllocationCallbacks* pAllocator,
                                             VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

inline void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                          const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

namespace String {
namespace Vulkan {

class VulkanDebugger {
private:
    VkDebugUtilsMessengerEXT debug_messenger_;
    VkInstance& instance_handle_;

public:
    VulkanDebugger(VkInstance& instance_handle_);
    ~VulkanDebugger() noexcept;

    auto getDebugMessengerCreateInfo() noexcept -> VkDebugUtilsMessengerCreateInfoEXT;

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                        VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                        void* pUserData) {
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

        return VK_FALSE;
    }
};

inline VulkanDebugger::VulkanDebugger(VkInstance& instance_handle) : instance_handle_(instance_handle) {
    VkDebugUtilsMessengerCreateInfoEXT createInfo = getDebugMessengerCreateInfo();

    if (CreateDebugUtilsMessengerEXT(instance_handle, &createInfo, nullptr, &debug_messenger_) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug messenger!");
    }
}

inline VulkanDebugger::~VulkanDebugger() noexcept {
    DestroyDebugUtilsMessengerEXT(instance_handle_, debug_messenger_, nullptr);
}

inline auto VulkanDebugger::getDebugMessengerCreateInfo() noexcept -> VkDebugUtilsMessengerCreateInfoEXT {
    return VkDebugUtilsMessengerCreateInfoEXT{.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
                                              .pNext = nullptr,
                                              .flags = 0,
                                              .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
                                              .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                                             VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                                             VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
                                              .pfnUserCallback = debugCallback,
                                              .pUserData = nullptr};
}

}  // namespace Vulkan
}  // namespace String
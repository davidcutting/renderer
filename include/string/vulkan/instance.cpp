#include <GLFW/glfw3.h>

#include <string/core/debug.hpp>
#include <string/vulkan/instance.hpp>
#include <vector>

namespace String {
namespace Vulkan {

namespace {

static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                    VkDebugUtilsMessageTypeFlagsEXT /*messageType*/,
                                                    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                    void* /*pUserData*/) {
    switch (messageSeverity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: {
            STRING_LOG_ERROR(pCallbackData->pMessage);
            break;
        }
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: {
            STRING_LOG_INFO(pCallbackData->pMessage);
            break;
        }
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: {
            STRING_LOG_WARN(pCallbackData->pMessage);
            break;
        }
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: {
            STRING_LOG_TRACE(pCallbackData->pMessage);
            break;
        }
        default:
            STRING_LOG_INFO(pCallbackData->pMessage);
    }
    return VK_FALSE;
}

}  // namespace

Instance::Instance(const std::string& application_name, const Version& application_version) {
    VkApplicationInfo application_info{
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = nullptr,  // TODO: Point to extension information
        .pApplicationName = application_name.c_str(),
        .applicationVersion =
            VK_MAKE_VERSION(application_version.major, application_version.minor, application_version.patch),
        .pEngineName = "String Engine",
        .engineVersion = VK_MAKE_VERSION(0, 0, 1),
        .apiVersion = VK_API_VERSION_1_3,
    };

    // Setup Vulkan extensions
    uint32_t extension_count = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

    std::vector<VkExtensionProperties> available_instance_extensions(extension_count);
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, available_instance_extensions.data());

    // Get the extensions used by the GLFW windowing library
    // TODO: Make generic to any windowing library
    std::vector<const char*> required_extensions;
    {
        uint32_t glfw_extension_count = 0;
        const char** glfw_extensions;
        glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

        for (uint32_t i = 0; i < glfw_extension_count; ++i) {
            required_extensions.emplace_back(glfw_extensions[i]);
        }
    }

    // Set up DebugUtils to get validation messages during instance creation
    // and reuse to create main debug messenger.
    VkDebugUtilsMessengerCreateInfoEXT debug_create_info{
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .pNext = nullptr,
        .flags = 0,
        .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
        .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                       VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
        .pfnUserCallback = debug_callback,
        .pUserData = nullptr,
    };

    // Create Vulkan Instance
    VkInstanceCreateInfo create_info{
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debug_create_info,
        .flags = 0,
        .pApplicationInfo = &application_info,
        .enabledLayerCount = (uint32_t)enabled_layers_.size(),
        .ppEnabledLayerNames = enabled_layers_.data(),
        .enabledExtensionCount = (uint32_t)enabled_extensions_.size(),
        .ppEnabledExtensionNames = enabled_extensions_.data(),
    };

    // TODO: Use custom allocator
    const auto instance_result = vkCreateInstance(&create_info, nullptr, &instance_handle_);
    STRING_ASSERT(instance_result == VK_SUCCESS);

    // Create vulkan debugger
    const auto debug_result =
        create_debug_utils_messenger_ext(instance_handle_, &debug_create_info, nullptr, &debug_utils_messenger_);
    STRING_ASSERT(debug_result == VK_SUCCESS);
}

Instance::Instance(VkInstance instance) : instance_handle_(instance) {}

Instance::~Instance() {
    destroy_debug_utils_messenger_ext(instance_handle_, debug_utils_messenger_, nullptr);
    if (debug_utils_messenger_ != VK_NULL_HANDLE) {
        vkDestroyDebugUtilsMessengerEXT(instance_handle_, debug_utils_messenger_, nullptr);
    }
    if (instance_handle_ != VK_NULL_HANDLE) {
        vkDestroyInstance(instance_handle_, nullptr);
    }
}

VkInstance Instance::get_handle() const noexcept { return instance_handle_; }

bool Instance::is_extension_enabled(const char* extension) const noexcept {
    return std::find_if(enabled_extensions_.begin(), enabled_extensions_.end(),
                        [extension](const char* enabled_extension) {
                            return strcmp(extension, enabled_extension) == 0;
                        }) != enabled_extensions_.end();
}

const std::vector<const char*>& Instance::get_extensions() const noexcept { return enabled_extensions_; }

VkResult Instance::create_debug_utils_messenger_ext(VkInstance instance,
                                                    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                                    const VkAllocationCallbacks* pAllocator,
                                                    VkDebugUtilsMessengerEXT* pDebugMessenger) noexcept {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void Instance::destroy_debug_utils_messenger_ext(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                                 const VkAllocationCallbacks* pAllocator) noexcept {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

}  // namespace Vulkan
}  // namespace String
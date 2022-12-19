#include <string/vulkan/instance.hpp>

namespace String {
namespace Vulkan {

Instance::Instance(const std::string& application_name, const Version& application_version) {
    VkApplicationInfo application_info{
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = nullptr,  // TODO: Point to extension information
        .pApplicationName = application_name.c_str(),
        .applicationVersion = VK_MAKE_VERSION(application_version.major, application_version.minor, application_version.patch),
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

Instance::~Instance() {
    // TODO: Use custom allocator
    if (instance_handle_ != VK_NULL_HANDLE) {
        vkDestroyInstance(instance_handle_, nullptr);
    }
    // TODO: Log
}

VkInstance Instance::get_handle() const noexcept { return instance_handle_; }

}  // namespace Vulkan
}  // namespace String

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
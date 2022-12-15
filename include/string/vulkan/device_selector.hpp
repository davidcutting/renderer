#pragma once

#include <vulkan/vulkan.h>

#include <optional>

namespace String {
namespace Vulkan {

enum DeviceType { DEDICATED_GRAPHICS };

class DeviceSelector {
private:
    /** @brief The GPU selected based off user provided criteria. */
    VkPhysicalDevice selected_device_ = VK_NULL_HANDLE;

    /** @brief The number of GPU's found on the host that support Vulkan.*/
    uint32_t num_devices_found_ = 0;

public:
    auto query_physical_devices() noexcept -> std::optional<DeviceSelector&> const&;
    auto filter_by_suitable() noexcept -> std::optional<DeviceSelector&> const&;
    auto filter_by_type(const DeviceType& type) noexcept -> std::optional<DeviceSelector&> const&;
    auto select() const noexcept -> std::optional<VkPhysicalDevice> const&;
};

}  // namespace Vulkan
}  // namespace String
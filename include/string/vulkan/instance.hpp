#pragma once

#include <vulkan/vulkan.h>
#include <cstdint>
#include <optional>

namespace String {
namespace Vulkan {

class InstanceBuilder {
private:
    VkInstance instance_handle_ = VK_NULL_HANDLE;
    uint32_t num_extensions_available = 0;

public:
    auto begin() noexcept -> std::optional<InstanceBuilder&> const&;
    auto set_application_version(const uint32_t& major, const uint32_t& minor, const uint32_t& patch) noexcept -> InstanceBuilder&
};

}  // namespace Vulkan
}  // namespace String
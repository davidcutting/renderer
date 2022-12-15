#pragma once

namespace String {
namespace Vulkan {

enum BufferType {
    VERTEX,
    INDEX,
    STAGING,
};

class Buffer {
private:
    Device& device_handle_;
public:
    // Create Vertex Buffer
    Buffer(const Device& device, const BufferType& type);
};

}  // namespace Vulkan
}  // namespace String
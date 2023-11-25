#pragma once

#include <array>
#include <bitset>
#include <cassert>
#include <cstdint>
#include <queue>

namespace String {

using Entity = std::uint32_t;
static constexpr uint32_t MAX_ENTITIES = 5000;

using ComponentType = std::uint8_t;
static constexpr uint32_t MAX_COMPONENTS = 32;

using Signature = std::bitset<MAX_COMPONENTS>;

}  // namespace String

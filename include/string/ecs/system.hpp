#pragma once

#include <bits/stdc++.h>

#include "entity.hpp"

namespace String {

class System {
public:
    std::set<Entity> entities;

    virtual ~System() = default;
    virtual auto update(float dt) -> void = 0;
};

}  // namespace String
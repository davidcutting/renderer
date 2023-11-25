#pragma once

#include <string/math/Vec2.hpp>
#include <string/math/Vec3.hpp>
#include <vector>

namespace String {

struct Vertex {
    Vec2 position;
    Vec3 color;
};

struct Mesh {
    std::vector<Vertex> vertices;
};

}
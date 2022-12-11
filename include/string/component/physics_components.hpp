#pragma once

#include <string/math/Vec3.hpp>

namespace String {

struct Gravity {
    Vec3 force;
};

struct RigidBody {
    Vec3 velocity;
    Vec3 acceleration;
};

struct Transform
{
	Vec3 position;
	Vec3 rotation;
	Vec3 scale;
};

struct Thrust
{
	Vec3 force;
};

}  // namespace String
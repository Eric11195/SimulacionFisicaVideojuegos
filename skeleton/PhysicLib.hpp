#pragma once 
#include "PxPhysicsAPI.h"
#include "core.hpp"

namespace PhysicLib {
	using mass = float;
	using speed = float;
	using gravity = float;

	constexpr gravity GRAVITY = 9.8;
	constexpr float NORMAL_DAMPING = 0.999;
	const physx::PxTransform NEUTRAL_TRANSFORM = physx::PxTransform(0,0,0);
}
#pragma once
#include "GameObject.hpp"

class BlackHole : public GameObject {
public:
	BlackHole(physx::PxScene* s, physx::PxVec3 pos_relative_to_parent, float force_module);
};

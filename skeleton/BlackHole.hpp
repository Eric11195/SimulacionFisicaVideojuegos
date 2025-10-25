#pragma once
#include "GameObject.hpp"

class BlackHole : public GameObject {
public:
	BlackHole(physx::PxVec3 pos_relative_to_parent, float force_module);
};

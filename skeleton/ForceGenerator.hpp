#pragma once
#include "GameObject.hpp"
using namespace physx;

class ForceGenerator : public GameObject {
public:
	virtual physx::PxVec3 apply_force(GameObject& g) = 0;
protected:
	PxVec3 local_force;
	PxVec3 global_force;
};

class Directional_ForceGenerator : public ForceGenerator {
	virtual physx::PxVec3 apply_force(GameObject& g) override;
	virtual void update_position(Transform const& parent_tr) override;
};
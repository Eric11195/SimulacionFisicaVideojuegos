#pragma once
#include "GameObject.hpp"

class ForceGenerator : public GameObject {
public:
	ForceGenerator(std::string name);
	~ForceGenerator();
	virtual physx::PxVec3 apply_force(GameObject const& g) = 0;
protected:
	physx::PxVec3 local_force;
	physx::PxVec3 global_force;
	std::string my_name;
};

class Directional_ForceGenerator : public ForceGenerator {
public:
	Directional_ForceGenerator(std::string s, physx::PxVec3 force_direction, float force_magnitude);
	virtual physx::PxVec3 apply_force(GameObject const& g) override;
	virtual void update_position(Transform const& parent_tr) override;
};

class Gravity_ForceGenerator : public Directional_ForceGenerator {
public:
	Gravity_ForceGenerator(physx::PxVec3 force_direction);
	virtual physx::PxVec3 apply_force(GameObject const& g) override;
};


class Wind_ForceGenerator : public Directional_ForceGenerator {
public:
	Wind_ForceGenerator(std::string s, physx::PxVec3, float magnitude);
	virtual physx::PxVec3 apply_force(GameObject const& g) override;
};
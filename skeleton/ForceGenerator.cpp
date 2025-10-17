#include "ForceGenerator.hpp"

//F = masa * accel
physx::PxVec3 Directional_ForceGenerator::apply_force(GameObject& g)
{
	return global_force / g.get_mass();
}

void Directional_ForceGenerator::update_position(Transform const& parent_tr)
{
	GameObject::update_position(parent_tr);
	global_force = parent_tr.rotate(local_force);
}

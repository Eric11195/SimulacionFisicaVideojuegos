#include "BlackHole.hpp"
#include "SceneObject.hpp"
#include "ForceGenerator.hpp"

BlackHole::BlackHole(physx::PxVec3 pos_relative_to_parent, float force_module)
	:GameObject()
{
	set_local_pos(pos_relative_to_parent);

	addChild(new SphereObject({ {GameObject::config(), {0,0,0,1}},0.5 }));
	addChild(new Torbellino_ForceGenerator("black_hole", {0,0,0}, force_module, 0.33, 0.50, Torbellino_ForceGenerator::y));
}
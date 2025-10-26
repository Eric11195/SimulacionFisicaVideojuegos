#include "BlackHole.hpp"
#include "SceneObject.hpp"
#include "ForceGenerator.hpp"

BlackHole::BlackHole(physx::PxVec3 pos_relative_to_parent, float force_module)
	:GameObject()
{
	set_local_pos(pos_relative_to_parent);

	addChild(new SphereObject({ {GameObject::config(), {0,0,0,1}},0.5 }));
	addChild(new TorbellinoSencillo("black_hole", {0,0,1}, force_module));
}
#include "BlackHole.hpp"
#include "SceneObject.hpp"
#include "ForceGenerator.hpp"

BlackHole::BlackHole(physx::PxScene* s, physx::PxVec3 pos_relative_to_parent, float force_module)
	:GameObject(s)
{
	global_transform.p = pos_relative_to_parent;
	SphereObject::config sphere_c = { {GameObject::config(), {0,0,0,1}},0.5 };
	auto sph = new SphereObject(s, sphere_c);
	sph->setTransform(global_transform);
	addChild(sph);
	auto f = new TorbellinoSencillo(s,"black_hole", { 0,0,0 }, 10, 20);
	f->setTransform(global_transform);
	addChild(f);
	//addChild(new Torbellino_ForceGenerator("black_hole", {0,0,0}, force_module, 0.33, 0.50, Torbellino_ForceGenerator::y));
}
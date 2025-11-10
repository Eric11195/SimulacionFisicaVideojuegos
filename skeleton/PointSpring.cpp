#include "PointSpring.hpp"

PointSpring::PointSpring(physx::PxVec3 v, config c, std::string name)
	: PT_OBJ_Spring_ForceGenerator(name, c)
{
	translate_to(v);
	CubeObject::config cube_c = {
		SceneObject::config({ GameObject::config(), Color{v.x,v.y,v.z,1} }),
		{0.5,0.5,0.5}
	};
	cube_c.so_config.go_config.pos = v;
	new CubeObject(cube_c);
}

#include "SceneObject.hpp"
#include <cassert>


void SceneObject::set_color(Color c)
{
	render_item->color = c;
}

SceneObject::SceneObject(physx::PxScene* s, PxShape* shape, config& c)
	:GameObject(s,c.go_config), render_item(std::make_unique<RenderItem>(shape, &global_transform, c.color))
{
	//render_item = std::make_unique<RenderItem>(shape,&global_transform, c.color);
	//RegisterRenderItem(render_item.get());
}

SceneObject::SceneObject(physx::PxScene* s, config& c)
	: GameObject(s,c.go_config), render_item(nullptr)
{
}

SceneObject::SceneObject(physx::PxScene* s, PxShape* shape, PxRigidActor* actor, config& c)
	: GameObject(s, c.go_config), render_item(std::make_unique<RenderItem>(shape, actor, c.color))
{
	assert(actor);
}

SceneObject::~SceneObject()
{
	if((render_item)!=nullptr)
		render_item->release();
}

void SceneObject::render3D()
{
	GameObject::render3D();
	render_go::render3D(render_item.get());
}


SphereObject::SphereObject(physx::PxScene* s, config& c)
	: SceneObject(s,CreateShape(PxSphereGeometry(c.radius)), c.so_config) {}
/*
SphereObject::SphereObject(SceneObject::config c, PxShape* shape)
	:SceneObject(c, shape) {}
*/

CubeObject::CubeObject(physx::PxScene* s, config& c)
	:SceneObject(s, CreateShape(PxBoxGeometry(c.half_extents)), c.so_config) {}

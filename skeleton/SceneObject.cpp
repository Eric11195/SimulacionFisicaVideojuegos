#include "SceneObject.hpp"


SceneObject::SceneObject(config c, PxShape* shape)
	:sh(shape),GameObject(c.go_config)
{
	render_item = new RenderItem(sh,&global_transform, c.color);
	RegisterRenderItem(render_item);
}

SceneObject::~SceneObject()
{
	DeregisterRenderItem(render_item);
	delete render_item;
}


SphereObject::SphereObject(config c)
	:SceneObject(c.so_config, CreateShape(PxSphereGeometry(c.radius))) {}
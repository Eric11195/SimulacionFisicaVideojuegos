#include "SceneObject.hpp"


SceneObject::SceneObject(config c, PxShape* shape)
	:GameObject(c.go_config)
{
	render_item = std::make_unique<RenderItem>(shape,&global_transform, c.color);
	//RegisterRenderItem(render_item.get());
}

SceneObject::~SceneObject()
{
	(*render_item).release();
}


SphereObject::SphereObject(config c)
	:SceneObject(c.so_config, CreateShape(PxSphereGeometry(c.radius))) {}
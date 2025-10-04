#include "SceneObject.hpp"

SceneObject::SceneObject(PxShape* _sh, My_Vector3 v, Color _cl)
	: sh(_sh), cl(_cl), GameObject(Transform(v.turn())) {
	render_item = new RenderItem(sh, &global_transform, cl);
	RegisterRenderItem(render_item);
}

SceneObject::~SceneObject()
{
	DeregisterRenderItem(render_item);
	delete render_item;
}

SphereObject::SphereObject(const float rad, My_Vector3 _pos, Color c)
	: SceneObject(CreateShape(PxSphereGeometry(rad)), _pos, c) {
}
#include "SceneObject.hpp"

SceneObject::SceneObject(PxShape* _sh, My_Vector3 v, Color _cl)
	: sh(_sh), tr(v.turn()), cl(_cl) {
	render_item = new RenderItem(sh, &tr, cl);
	RegisterRenderItem(render_item);
}

SceneObject::~SceneObject()
{
	DeregisterRenderItem(render_item);
}

SphereObject::SphereObject(const float rad, My_Vector3 _pos, Color c)
	: SceneObject(CreateShape(PxSphereGeometry(rad)), _pos, c) {
}
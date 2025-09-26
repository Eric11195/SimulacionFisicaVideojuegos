#include "GameObject.hpp"

GameObject::GameObject(PxShape* _sh, My_Vector3 v, Color _cl)
	: sh(_sh), tr(v.turn()), cl(_cl) {
	render_item = new RenderItem(sh, &tr, cl);
	RegisterRenderItem(render_item);
	my_it = gameobject_list.insert(gameobject_list.end(), this);
}

GameObject::~GameObject()
{
	DeregisterRenderItem(render_item);
}

void GameObject::cleanup()
{
	gameobject_list.erase(my_it);
	delete this;
	//delete render_item;
}

void GameObject::step_all(double dt)
{
	for (auto obj : gameobject_list)
		obj->step(dt);
}

void GameObject::release_all()
{
	for (auto obj : gameobject_list)
		delete obj;
	gameobject_list.clear();
}

SphereObject::SphereObject(const float rad, My_Vector3 _pos, Color c)
	: GameObject(CreateShape(PxSphereGeometry(rad)), _pos, c) {
}
#include "GameObject.hpp"
#include "CompositeGameObject.hpp"

GameObject::GameObject(Transform t)//, std::list<GameObject*> l)
	:local_transform(t), global_transform(t)
{
	//my_it = l.insert(l.end(), this);
}

void GameObject::cleanup() {
	//Parent will erase them of the list if they are there
	delete this;
}
void GameObject::translate(physx::PxVec3 t)
{
	translate_to(local_transform.p + t);
}
void GameObject::translate_to(physx::PxVec3 t)
{
	local_transform.p = t;
}
void GameObject::link_to_parent(Transform const& parent_tr)
{
	update_position(parent_tr);
}
void GameObject::update_position(Transform const& parent_tr)
{
	global_transform = local_transform * parent_tr;
}
/*
void GameObject::step_all(double dt)
{
	for (auto obj : GameObject_list)
		obj->step(dt);
}

void GameObject::release_all()
{
	for (auto obj : GameObject_list)
		delete obj;
	GameObject_list.clear();
}

void GameObject::process_input_all(unsigned char key)
{
	for (auto obj : GameObject_list)
		obj->process_input(key);
}
*/
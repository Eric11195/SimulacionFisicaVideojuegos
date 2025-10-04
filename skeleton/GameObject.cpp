#include "GameObject.hpp"
#include "CompositeGameObject.hpp"

GameObject::GameObject(Transform t)//, std::list<GameObject*> l)
	:tr(t)
{
	//my_it = l.insert(l.end(), this);
}

void GameObject::cleanup() {
	//Parent will erase them of the list if they are there
	delete this;
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
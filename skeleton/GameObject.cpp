#include "GameObject.hpp"

GameObject::GameObject()
{
	my_it = GameObject_list.insert(GameObject_list.end(), this);
}

void GameObject::cleanup() {
	GameObject_list.erase(my_it);
	delete this;
	//delete render_item;
}
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

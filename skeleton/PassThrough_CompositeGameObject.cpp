#include "PassThrough_CompositeGameObject.hpp"

PassThrough_CompositeGameObject::PassThrough_CompositeGameObject(config& c, Transform const& parent_tr, std::initializer_list<GameObject*> go_s)
	:GameObject(c,go_s), parent_transform(parent_tr)
{
}

void PassThrough_CompositeGameObject::step(double dt)
{
	integrate(dt);

	for (auto& child : child_objects) {
		child->step(dt);
	}
}

void PassThrough_CompositeGameObject::update_position(Transform const& parent)
{
	global_transform = parent.transform(local_transform);
	for (auto& child : child_objects)
		child->update_position(parent_transform);
}

std::list<std::unique_ptr<GameObject>>::iterator PassThrough_CompositeGameObject::addChild(GameObject* go)
{
	auto child = child_objects.insert(child_objects.end(), std::unique_ptr<GameObject>(go));
	(*child)->update_position(parent_transform);
	return child;
}

#include "CompositeGameObject.hpp"

CompositeGameObject::CompositeGameObject(GameObject::config c)
    :GameObject(c)
{
}

void CompositeGameObject::addChild(GameObject* go)
{
    auto aux_it = child_objects.insert(child_objects.end(),go);
    (*aux_it)->link_to_parent(global_transform);
}

void CompositeGameObject::step(double dt)
{
    for (auto child : child_objects) {
        child->step(dt);
        child->update_position(global_transform);
    }
}

//No game object can destroy itself, only parents can destroy their childs-----------------------------------------------------------
void CompositeGameObject::cleanup()
{
    //delete all childs
    for (auto child : child_objects)
        child->cleanup();
    //Then delete me
    delete this;
    //The same as doing GameObject::cleanup()
}

void CompositeGameObject::process_input(unsigned char key)
{
    for (auto child : child_objects)
        child->process_input(key);

}

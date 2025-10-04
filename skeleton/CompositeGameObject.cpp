#include "CompositeGameObject.hpp"

CompositeGameObject::CompositeGameObject(Transform t)
    :GameObject(t)
{
}

void CompositeGameObject::addChild(GameObject* go)
{
    //auto aux_it = 
    child_objects.insert(child_objects.end(),go);
    //go->link_to_parent(aux_it);
}

void CompositeGameObject::step(double dt)
{
    for (auto child : child_objects)
        child->step(dt);
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

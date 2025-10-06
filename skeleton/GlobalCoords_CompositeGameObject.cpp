#include "GlobalCoords_CompositeGameObject.hpp"
#include "PhysicLib.hpp"

GlobalCoords_CompositeGameObject::GlobalCoords_CompositeGameObject(config& c)
    :CompositeGameObject(c)
{
}

void GlobalCoords_CompositeGameObject::step(double dt)
{

    for (auto& child : child_objects) {
        child->step(dt);
        child->update_position(PhysicLib::NEUTRAL_TRANSFORM);
    }
    GameObject::step(dt);
}

void GlobalCoords_CompositeGameObject::addChild(GameObject* go)
{
    auto child = child_objects.insert(child_objects.end(), std::unique_ptr<GameObject>(go));
    (*child)->update_position(PhysicLib::NEUTRAL_TRANSFORM);
}

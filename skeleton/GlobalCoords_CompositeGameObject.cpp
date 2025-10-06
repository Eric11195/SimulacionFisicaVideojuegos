#include "GlobalCoords_CompositeGameObject.hpp"
#include "PhysicLib.hpp"

GlobalCoords_CompositeGameObject::GlobalCoords_CompositeGameObject(config c)
    :CompositeGameObject(c)
{
}

void GlobalCoords_CompositeGameObject::step(double dt)
{
    for (auto child : child_objects) {
        child->step(dt);
        child->update_position(PhysicLib::NEUTRAL_TRANSFORM);
    }
}

void GlobalCoords_CompositeGameObject::addChild(GameObject* go)
{
    auto child = child_objects.insert(child_objects.end(), go);
    (*child)->update_position(PhysicLib::NEUTRAL_TRANSFORM);
}

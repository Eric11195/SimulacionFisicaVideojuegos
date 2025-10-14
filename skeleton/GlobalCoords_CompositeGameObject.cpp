#include "GlobalCoords_CompositeGameObject.hpp"
#include "PhysicLib.hpp"

GlobalCoords_CompositeGameObject::GlobalCoords_CompositeGameObject(config& c, std::initializer_list<GameObject*> go_s)
    :GameObject(c,go_s)
{
}

void GlobalCoords_CompositeGameObject::step(double dt)
{
    integrate(dt);

    for (auto& child : child_objects) {
        child->step(dt);
        child->update_position(PhysicLib::NEUTRAL_TRANSFORM);
    }
}

void GlobalCoords_CompositeGameObject::addChild(GameObject* go)
{
    auto child = child_objects.insert(child_objects.end(), std::unique_ptr<GameObject>(go));
    (*child)->update_position(PhysicLib::NEUTRAL_TRANSFORM);
}

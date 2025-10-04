#include "GlobalCoords_CompositeGameObject.hpp"


GlobalCoords_CompositeGameObject::GlobalCoords_CompositeGameObject()
    : CompositeGameObject(Transform(0,0,0)){
}

/*
void GlobalCoords_CompositeGameObject::step(double dt)
{
    for (auto child : child_objects) {
        child->step(dt);
        child->update_position(global_transform);
    }
}
*/

//This will do nothing
void GlobalCoords_CompositeGameObject::update_position(Transform& parent_tr)
{
}

//This will do nothing
void GlobalCoords_CompositeGameObject::translate_to(physx::PxVec3)
{
}

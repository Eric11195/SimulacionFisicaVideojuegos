#pragma once
#include "CompositeGameObject.hpp"

//If someone tries to move this gameobject it will change_nothing

//Its transform does not affect the objects inside it
class GlobalCoords_CompositeGameObject : public CompositeGameObject {
public:
	//Its transform its always the the default, no matter what
	GlobalCoords_CompositeGameObject();
	//virtual void step(double dt) override;
	virtual void update_position(Transform& parent_tr) override;
	virtual void translate_to(physx::PxVec3) override;
};
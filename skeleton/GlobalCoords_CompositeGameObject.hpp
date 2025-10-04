#pragma once
#include "CompositeGameObject.hpp"
#include "PhysicLib.hpp"

//If someone tries to move this gameobject it will change_nothing

//Its transform does not affect the objects inside it
class GlobalCoords_CompositeGameObject : public CompositeGameObject {
public:
	//Its transform its always the the default, no matter what
	GlobalCoords_CompositeGameObject(Transform tr = PhysicLib::NEUTRAL_TRANSFORM);
	virtual void step(double dt) override;
	virtual void addChild(GameObject* go) override;
};
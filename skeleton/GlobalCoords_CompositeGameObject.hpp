#pragma once
#include "GameObject.hpp"
#include "PhysicLib.hpp"

//If someone tries to move this gameobject it will change_nothing

//Its transform does not affect the objects inside it
class GlobalCoords_CompositeGameObject : public GameObject {
public:
	GlobalCoords_CompositeGameObject(const GlobalCoords_CompositeGameObject&) = delete;
	GlobalCoords_CompositeGameObject& operator =(const GlobalCoords_CompositeGameObject&) = delete;
	//Its transform its always the the default, no matter what
	GlobalCoords_CompositeGameObject(config& = config(), std::initializer_list<GameObject*> = {});
	virtual void step(double dt) override;
	virtual std::list<std::unique_ptr<GameObject>>::iterator addChild(GameObject* go) override;
};
#pragma once
#include "SceneObject.hpp"
#include "GlobalCoords_CompositeGameObject.hpp"
#include "PhysicLib.hpp"

class CoordinateAxis : public GlobalCoords_CompositeGameObject{
public:
	CoordinateAxis() 
		: GlobalCoords_CompositeGameObject(PhysicLib::NEUTRAL_TRANSFORM){
		addChild(new SphereObject(1));
		addChild(new SphereObject(1, My_Vector3(5, 0, 0), Color(1, 0, 0, 1)));
		addChild(new SphereObject(1, My_Vector3(0, 5, 0), Color(0, 1, 0, 1)));
		addChild(new SphereObject(1, My_Vector3(0, 0, 5), Color(0, 0, 1, 1)));
	}
};
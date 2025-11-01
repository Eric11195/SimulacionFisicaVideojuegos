#pragma once
#include "SceneObject.hpp"
#include "PhysicLib.hpp"

class CoordinateAxis : public GameObject{
public:
	CoordinateAxis() 
		: GameObject(GameObject::config()){
		//NEUTRAL WHITE BALL
		addChild(new SphereObject(SphereObject::config()));
		//RED X RIGHT
		addChild(new SphereObject
		(SphereObject::config{//Scene
			SceneObject::config{//GO
				GameObject::config{{5,0,0}},
				Color(1, 0, 0, 1)
			},
			1
		}));
		//GREEN Y UP
		addChild(new SphereObject
		(SphereObject::config{//Scene
			SceneObject::config{//GO
				GameObject::config{{0,5,0}},
				Color(0, 1, 0, 1)
			},
			1
		}));
		//BLUE Z FORWARD
		addChild(new SphereObject
		(SphereObject::config{//Scene
			SceneObject::config{//GO
				GameObject::config{{0,0,5}},
				Color(0, 0, 1, 1)
			},
			1
		}));
	}
};
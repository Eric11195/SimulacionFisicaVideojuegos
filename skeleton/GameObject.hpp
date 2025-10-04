#pragma once
#include <list>
#include "PxPhysicsAPI.h"

using Transform = physx::PxTransform;
class CompositeGameObject;

struct GameObject {
	GameObject(Transform t = Transform(0, 0, 0));//, std::list<GameObject*> l = GameObject_list);
	virtual void step(double dt) {}
	virtual void init() {}
	virtual void cleanup();
	virtual void process_input(unsigned char key) {}
	//They dont need to know who contains them
	//void link_to_parent(std::list<GameObject*>::iterator cgo);
	//-------------------------------------------------------
	//static void step_all(double dt);
	//static void release_all();
	//static void process_input_all(unsigned char key);
	//-------------------------------------------------------
protected:
	Transform tr;
};
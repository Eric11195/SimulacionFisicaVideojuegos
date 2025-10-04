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
	virtual void link_to_parent(Transform& parent_tr);
	virtual void update_position(Transform& parent_tr);
	void translate(physx::PxVec3);
	virtual void translate_to(physx::PxVec3);
protected:
	Transform global_transform;
	//Use only in calculations;
	Transform local_transform;
};
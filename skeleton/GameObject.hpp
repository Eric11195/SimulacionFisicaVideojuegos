#pragma once
#include "PxPhysicsAPI.h"
#include "My_Vector3.hpp"
#include "core.hpp"
#include "RenderUtils.hpp"
#include <list>

using Transform = physx::PxTransform;
using Color = Vector4;
using namespace physx;

class GameObject;

class GameObject {
protected:
	Transform tr;
	Color cl;
	RenderItem* render_item;
	//Shape
	PxShape* sh;
public:
	GameObject(PxShape* _sh, My_Vector3 v, Color _cl);
	~GameObject();

	virtual void step(double dt) {}
	virtual void init() {}
	virtual void cleanup();

	//-------------------------------------------------------
	static void step_all(double dt);
	static void release_all();
	//-------------------------------------------------------
private:
	inline static std::list<GameObject*> gameobject_list{};
	std::list<GameObject*>::iterator my_it;

};


struct SphereObject : GameObject{
	SphereObject(const float rad, My_Vector3 _pos = My_Vector3::zero(), Color c = Color(1, 1, 1, 1));
};
//TO DO:
//• PxBoxGeometry
//• PxCapsuleGeometry
//• PxPlaneGeometry
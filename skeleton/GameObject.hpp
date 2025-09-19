#pragma once
#include "PxPhysicsAPI.h"
#include "Vector3.hpp"

using Transform = const physx::PxTransform;
using Color = const Vector4;
using namespace physx;

class GameObject {
protected:
	RenderItem* render_item;
public:
	GameObject(RenderItem* r) : render_item(r) {
		RegisterRenderItem(render_item);
	}
	~GameObject() {
		DeregisterRenderItem(render_item);
		delete render_item;
	}
};


struct  SphereObject : GameObject{
	SphereObject(float rad, My_Vector3 v= My_Vector3::zero(),Color c=Color(1,1,1,1))
		: GameObject(new RenderItem(CreateShape(PxSphereGeometry(rad)), new Transform(v.x,v.y,v.z), Color(c.x,c.y,c.z,c.w))) { }
};
//TO DO:
//• PxBoxGeometry
//• PxCapsuleGeometry
//• PxPlaneGeometry
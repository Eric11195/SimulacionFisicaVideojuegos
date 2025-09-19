#pragma once
#include "PxPhysicsAPI.h"

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

	//SetPosition()
};


struct  SphereObject : GameObject{
	SphereObject(float rad) 
		: GameObject(new RenderItem(CreateShape(PxSphereGeometry(rad)), new Transform(0,0,0), Color(1,1,1,1))) { }
};
//TO DO:
//• PxBoxGeometry
//• PxCapsuleGeometry
//• PxPlaneGeometry
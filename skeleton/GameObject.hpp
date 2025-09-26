#pragma once
#include "PxPhysicsAPI.h"
#include "My_Vector3.hpp"
#include "core.hpp"
#include "RenderUtils.hpp"

using Transform = physx::PxTransform;
using Color = Vector4;
using namespace physx;

class GameObject {
protected:
	Transform tr;
	Color cl;
	RenderItem* render_item;
	//Shape
	PxShape* sh;
public:
	GameObject(PxShape* _sh, My_Vector3 v, Color _cl) : sh(_sh), tr(v.turn()), cl(_cl) {
		render_item = new RenderItem(sh,&tr,cl);
		RegisterRenderItem(render_item);
	}
	~GameObject() {
		DeregisterRenderItem(render_item);
		//delete render_item;
	}
};


struct SphereObject : GameObject{
	SphereObject(const float rad, My_Vector3 _pos = My_Vector3::zero(), Color c = Color(1, 1, 1, 1))
		: GameObject(CreateShape(PxSphereGeometry(rad)), _pos, c) { }
};
//TO DO:
//• PxBoxGeometry
//• PxCapsuleGeometry
//• PxPlaneGeometry
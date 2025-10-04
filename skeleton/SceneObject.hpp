#pragma once
#include "PxPhysicsAPI.h"
#include "My_Vector3.hpp"
#include "core.hpp"
#include "RenderUtils.hpp"
#include <list>
#include "GameObject.hpp"

using Transform = physx::PxTransform;
using Color = Vector4;
using namespace physx;

class SceneObject : public GameObject{
protected:
	Transform tr;
	Color cl;
	RenderItem* render_item;
	//Shape
	PxShape* sh;
public:
	//FOR DRAGON USE PxShape = Triangle Meshes
	SceneObject(PxShape* _sh, My_Vector3 v, Color _cl);
	~SceneObject();

	virtual void step(double dt) override {}
	virtual void init() override{}
	//virtual void cleanup() override;

};


struct SphereObject : SceneObject{
	SphereObject(const float rad, My_Vector3 _pos = My_Vector3::zero(), Color c = Color(1, 1, 1, 1));
};
//TO DO:
//• PxBoxGeometry
//• PxCapsuleGeometry
//• PxPlaneGeometry
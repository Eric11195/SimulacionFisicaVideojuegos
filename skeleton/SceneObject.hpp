#pragma once
#include "My_Vector3.hpp"
#include "core.hpp"
#include "RenderUtils.hpp"
#include <list>
#include "GameObject.hpp"

using Color = Vector4;
using namespace physx;

class SceneObject : public GameObject{
protected:
	Color cl;
	RenderItem* render_item;
	//Shape
	PxShape* sh;
public:
	struct config {
		GameObject::config go_config;
		Color color = Color(1, 1, 1, 1);
	};
	//FOR DRAGON USE PxShape = Triangle Meshes
	SceneObject(config c, PxShape* shape);
		//PxShape* _sh, My_Vector3 v, Color _cl);
	~SceneObject();

	//virtual void cleanup() override;

};


struct SphereObject : public SceneObject{
	struct config {
		SceneObject::config so_config;
		float radius = 1;
	};
	SphereObject(config c);
};
//TO DO:
//• PxBoxGeometry
//• PxCapsuleGeometry
//• PxPlaneGeometry
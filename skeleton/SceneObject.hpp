#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"
#include <list>
#include "GameObject.hpp"

using Color = Vector4;
using namespace physx;

class SceneObject : public GameObject{
protected:
	std::unique_ptr<RenderItem> render_item;
public:
	void set_color(Color c);
	struct config {
		GameObject::config go_config;
		Color color = Color(1, 1, 1, 1);
	};
	//FOR DRAGON USE PxShape = Triangle Meshes
	SceneObject(config c, PxShape* shape);
	~SceneObject() override;
};


struct SphereObject : public SceneObject{
	struct config {
		SceneObject::config so_config;
		float radius = 1;
	};
	SphereObject(config c);
	//SphereObject(SceneObject::config c, PxShape* shape);
};
struct CubeObject : public SceneObject {
	struct config {
		SceneObject::config so_config;
		PxVec3 half_extents;
	};
	CubeObject(config c);
	//CubeObject(SceneObject::config c, PxShape* shape);
};
//TO DO:
//• PxBoxGeometry
//• PxCapsuleGeometry
//• PxPlaneGeometry
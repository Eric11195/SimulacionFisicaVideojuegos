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
	SceneObject(physx::PxScene* s, PxShape* shape, config& c=config());
	SceneObject(physx::PxScene* s, config& c);
	SceneObject(physx::PxScene* s, PxShape* shape, PxRigidActor* actor, config& c);
	SceneObject(physx::PxScene* s, PxRigidActor* actor, config& c);
	~SceneObject() override;
	virtual void render3D() override;
private:
	void render_shape(const PxShape& shape, const PxTransform& transform, const PxVec4& color);
	void render_geometry(const PxGeometryHolder& h, bool wireframe = false);
};


struct SphereObject : public SceneObject{
	struct config {
		SceneObject::config so_config;
		float radius = 1;
	};
	SphereObject(physx::PxScene* s, config& c);
	//SphereObject(SceneObject::config c, PxShape* shape);
};
struct CubeObject : public SceneObject {
	struct config {
		SceneObject::config so_config;
		PxVec3 half_extents;
	};
	CubeObject(physx::PxScene* s, config& c);
	//CubeObject(SceneObject::config c, PxShape* shape);
};
//TO DO:
//• PxBoxGeometry
//• PxCapsuleGeometry
//• PxPlaneGeometry
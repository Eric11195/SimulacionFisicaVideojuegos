#pragma once

#include "SceneObject.hpp"

class Rigidbody_Object : public GameObject {
public:
	struct config {
		GameObject::config go_config;
	};
	Rigidbody_Object(PxScene* s, PxShape* sh,config& c);
	virtual void set_velocity(physx::PxVec3) override;
	PxRigidDynamic* set_rb(PxRigidDynamic* new_rb);
	virtual void set_pos(physx::PxVec3) override;
	void render3D() override;
	void step(double dt) override;
protected:
	PxRigidDynamic* rb;
	RenderItem* r_it;
};

class Rigid_SphereObject : public Rigidbody_Object {
public:
	struct config {
		Rigidbody_Object::config rb_config;
		float radius;
	};
	Rigid_SphereObject(PxScene* s, config& c);
};

struct Rigid_CubeObject : public Rigidbody_Object {
public:
	struct config {
		Rigidbody_Object::config rb_config;
		PxVec3 half_extents;
	};
	Rigid_CubeObject(physx::PxScene* s, config& c);
	//CubeObject(SceneObject::config c, PxShape* shape);
};

//STATIC------------------------------------------------------------------------------------------------------------

class StaticRigidbody_Object : public GameObject {
public:
	struct config {
		GameObject::config go_config;
	};
	StaticRigidbody_Object(PxScene* s, PxShape* sh, config& c);
	virtual void set_velocity(physx::PxVec3);
	PxRigidStatic* set_rb(PxRigidStatic*);
	void render3D() override;
protected:
	PxRigidStatic* rb;
	RenderItem* r_it;
};

class StaticRigid_SphereObject : public StaticRigidbody_Object {
public:
	struct config {
		StaticRigidbody_Object::config rb_config;
		float radius;
	};
	StaticRigid_SphereObject(PxScene* s, config& c);
};

struct StaticRigid_CubeObject : public StaticRigidbody_Object {
public:
	struct config {
		StaticRigidbody_Object::config rb_config;
		PxVec3 half_extents;
	};
	StaticRigid_CubeObject(physx::PxScene* s, config& c);
	//CubeObject(SceneObject::config c, PxShape* shape);
};
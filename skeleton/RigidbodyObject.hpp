#pragma once

#include "SceneObject.hpp"

class Rigidbody_Object : public SceneObject {
public:
	struct config {
		SceneObject::config so_config;
	};
	Rigidbody_Object(PxScene* s, PxShape* sh,config& c);
	virtual void set_velocity(physx::PxVec3) override;
	PxRigidDynamic* set_rb(PxRigidDynamic* new_rb);
	virtual void set_pos(physx::PxVec3) override;
	virtual Vector3 get_pos() override;
	virtual Vector3 get_vel() override;
	virtual void translate(physx::PxVec3) override;
	virtual void translate_to(physx::PxVec3) override;
	virtual void set_velocity_magnitude(float m) override;
	Transform get_global_tr();
	virtual void add_speed(physx::PxVec3) override;
protected:
	virtual void integrate(double dt) override;
	PxRigidDynamic* rb;
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

class StaticRigidbody_Object : public SceneObject {
public:
	struct config {
		SceneObject::config so_config;
	};
	StaticRigidbody_Object(PxScene* s, PxShape* sh, config& c);
	virtual void set_velocity(physx::PxVec3);
	PxRigidStatic* set_rb(PxRigidStatic*);
protected:
	PxRigidStatic* rb;
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
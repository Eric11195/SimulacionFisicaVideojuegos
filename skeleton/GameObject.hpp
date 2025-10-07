#pragma once
#include <list>
#include "PxPhysicsAPI.h"
#include "My_Vector3.hpp"
#include "PhysicLib.hpp"

#define EULER_SEMI_EXPLICIT_INTEGRATION
//#define EULER_INTEGRATION
#define DAMPING

using Transform = physx::PxTransform;
class CompositeGameObject;

struct GameObject {
	struct config {
		My_Vector3 pos = My_Vector3::zero(), initial_speed_dir = { 0,1,0 }, initial_accel_dir = {0,-1,0};
		float initial_speed_magnitude = 0, initial_accel_magnitude = 0;
		float damping_mult = PhysicLib::NORMAL_DAMPING;
	};
	GameObject(config& c = config());//, std::list<GameObject*> l = GameObject_list);
	virtual ~GameObject() {}

	virtual void step(double dt);
	virtual void init() {}
	virtual void cleanup();
	virtual void process_input(unsigned char key) {}
	virtual void link_to_parent(Transform const& parent_tr);
	virtual void update_position(Transform const& parent_tr);
	void translate(physx::PxVec3);
	virtual void translate_to(physx::PxVec3);

	void set_accel(My_Vector3 new_accel);
	void add_accel(My_Vector3 add_accel);
#ifdef DAMPING
	void set_dumping(float f);
#endif
protected:
	void integrate(double t);
	Transform global_transform;
	//Use only in calculations;
	Transform local_transform;
private:
#ifdef DAMPING
	float damping_mult;
#endif
	My_Vector3 vel;
	My_Vector3 accel;
};
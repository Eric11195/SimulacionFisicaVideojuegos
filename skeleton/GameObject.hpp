#pragma once
#include <list>
#include "PxPhysicsAPI.h"
#include "My_Vector3.hpp"
#include "PhysicLib.hpp"
#include "core.hpp"
#include "InputProcessor.hpp"
#include <memory>

#define EULER_SEMI_EXPLICIT_INTEGRATION
//#define EULER_INTEGRATION
#define DAMPING

using Transform = physx::PxTransform;
using Quaternion = physx::PxQuat;
class CompositeGameObject;

struct GameObject : public InputProcessor{
	struct config {
		My_Vector3 pos = My_Vector3::zero(), initial_speed_dir = { 0,1,0 }, initial_accel_dir = {0,-1,0};
		float initial_speed_magnitude = 0, initial_accel_magnitude = 0;
		float damping_mult = PhysicLib::NORMAL_DAMPING;
		Quaternion initial_rotation = Quaternion(physx::PxIDENTITY::PxIdentity);
	};
	GameObject(const CompositeGameObject&) = delete;
	GameObject& operator =(const CompositeGameObject&) = delete;
	GameObject(config& c = config(), std::initializer_list<GameObject*> go_s = {});
	virtual ~GameObject();

	virtual void addChild(GameObject* go);

	virtual Vector3 get_pos();
	virtual void step(double dt);
	virtual void init() {};
	//virtual void process_input(unsigned char key) {}
	virtual void link_to_parent(Transform const& parent_tr);
	virtual void update_position(Transform const& parent_tr);
	void translate(physx::PxVec3);
	virtual void translate_to(physx::PxVec3);
	virtual void rotate(physx::PxQuat);

	void set_accel(My_Vector3 new_accel);
	void add_accel(My_Vector3 add_accel);
	void set_vel(My_Vector3 vel);

	virtual void handle_mouse_pos(int x, int y) override;
	virtual void handle_mouse_button_up(uint8_t mb_id) override;
	virtual void handle_mouse_button_down(uint8_t mb_id) override;
	virtual void handle_keyboard_button_down(unsigned char key) override;
	virtual void handle_keyboard_button_up(unsigned char key) override;
#ifdef DAMPING
	void set_dumping(float f);
#endif
//protected:
	void integrate(double t);
	Transform global_transform;
	//Use only in calculations;
	Transform local_transform;
	std::list<std::unique_ptr<GameObject>> child_objects;

private:
#ifdef DAMPING
	float damping_mult;
#endif
	My_Vector3 vel;
	My_Vector3 accel;
};
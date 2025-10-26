#pragma once
#include <list>
#include "PxPhysicsAPI.h"
#include "PhysicLib.hpp"
#include "core.hpp"
#include "InputProcessor.hpp"
#include <memory>
#include <map>
#include <string>
#include "inverse_mass.hpp"

#define EULER_SEMI_EXPLICIT_INTEGRATION
//#define EULER_INTEGRATION
#define DAMPING

using Transform = physx::PxTransform;
using Quaternion = physx::PxQuat;
class ForceGenerator;

struct GameObject : public InputProcessor{
	inline static std::map<std::string, ForceGenerator*> force_generators_map = {};
	struct config {
		physx::PxVec3 pos = { 0,0,0 };
		physx::PxVec3 initial_speed_dir = { 0,1,0 };//, initial_accel_dir = { 0,-1,0 };
		float initial_speed_magnitude = 0;// , initial_accel_magnitude = 0;
		InvMass inverse_mass = InvMass(1);
		float damping_mult = PhysicLib::NORMAL_DAMPING;
		Quaternion initial_rotation = Quaternion(physx::PxIDENTITY::PxIdentity);
	};
	GameObject(const GameObject&) = delete;
	GameObject& operator =(const GameObject&) = delete;
	GameObject(config& c = config(), std::initializer_list<GameObject*> go_s = {});
	virtual ~GameObject();

	virtual std::list<std::unique_ptr<GameObject>>::iterator addChild(GameObject* go);

	void set_local_pos(physx::PxVec3 new_local_p) { local_transform.p = new_local_p; }
	virtual Vector3 get_pos();
	virtual Vector3 get_vel() const;
	virtual void step(double dt);
	virtual void init() {};
	//virtual void process_input(unsigned char key) {}
	virtual void link_to_parent(Transform const& parent_tr);
	virtual void update_position(Transform const& parent_tr);
	void translate(physx::PxVec3);
	virtual void translate_to(physx::PxVec3);
	virtual void rotate(Quaternion);

	void set_velocity(physx::PxVec3);
	void set_vel(physx::PxVec3 vel);
	float get_inv_mass() const { return m.inv_mass; }
	Transform get_global_tr_inverse() { return global_transform.getInverse(); }
	Transform get_global_tr()const { return global_transform; }

	virtual void handle_mouse_pos(float x, float y) override;
	virtual void handle_mouse_button_up(uint8_t mb_id) override;
	virtual void handle_mouse_button_down(uint8_t mb_id) override;
	virtual void handle_keyboard_button_down(unsigned char key) override;
	virtual void handle_keyboard_button_up(unsigned char key) override;

	void add_force_to_myself(ForceGenerator*);
	void add_force_to_myself(std::string);

#ifdef DAMPING
	void set_dumping(float f);
#endif
protected:
	void calculate_global_to_local_rot();
	void integrate(double t);
	InvMass m;
	Transform global_transform;
	//Use only in calculations;
	Transform local_transform;
	Quaternion global_to_local_rot;
	std::list<std::unique_ptr<GameObject>> child_objects;
	std::list<ForceGenerator*> forces_applied_to_this_obj;
	physx::PxVec3 vel;
private:
#ifdef DAMPING
	float damping_mult;
#endif
};
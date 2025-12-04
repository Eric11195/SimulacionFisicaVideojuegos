#pragma once

#include "RigidbodyObject.hpp"
#include "core.hpp"
#include "ShipInterface.hpp"

class Directional_ForceGenerator;
class TriggeredParticleGenerator;

class EnemyShip : public Rigid_CubeObject, public ShipInterface {
public:
	EnemyShip(physx::PxScene* s, GameObject*);
	virtual void step(double dt) override;
	void handle_keyboard_button_down(unsigned char c) override;
	virtual void set_collision_point(physx::PxVec3 pos, physx::PxVec3 normal) override;
	virtual bool alive() override;
protected:
	TriggeredParticleGenerator* explosion;
	bool to_be_destroyed = false;
	bool dead = false;
	bool exploded = false;
	double time_till_definitive_dead = 3;
	//physx::PxScene* my_scene;
	struct fire_transform {
		physx::PxVec3 pos;
		physx::PxQuat looking_to;
	};
	std::vector<fire_transform> points_impacted{};
	physx::PxVec3 think_off_torque();
	void think_step(double dt);
	GameObject* player_go;
	physx::PxTransform parent_to_child_tr;
	Directional_ForceGenerator* propulsors;
};
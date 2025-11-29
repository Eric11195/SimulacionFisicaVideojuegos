#pragma once

#include "RigidbodyObject.hpp"
#include "core.hpp"

class Directional_ForceGenerator;

class EnemyShip : public Rigid_CubeObject {
public:
	EnemyShip(physx::PxScene* s, GameObject*);
	virtual void step(double dt) override;
	void handle_keyboard_button_down(unsigned char c) override;
protected:
	physx::PxVec3 think_off_torque();
	void think_step(double dt);
	GameObject* player_go;
	physx::PxTransform parent_to_child_tr;
	Directional_ForceGenerator* propulsors;
};
#pragma once

#include "GameObject.hpp"
#include "core.hpp"

class EnemyShip : public GameObject {
public:
	EnemyShip(GameObject*);
	virtual void step(double dt) override;
protected:
	void think_step(double dt);
	GameObject* player_go;
	physx::PxTransform parent_to_child_tr;
};
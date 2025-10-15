#pragma once

#include "ParticleSystem.hpp"
//#include "ParticleGeneratorsDescriptors.hpp"

class ShipCannon : public ParticleSystem {
public:
	ShipCannon(Transform const& parent_tr);
	virtual void step(double dt) override;
	void start_fire();
	void stop_fire();
	void fire_missile();
protected:
	bool active = false;
	virtual void normal_shoot();
	virtual void step_fire(double dt);
	std::list<std::unique_ptr<GameObject>>::iterator cannon_it;
	std::list<std::unique_ptr<GameObject>>::iterator missile_cannon;
	int n_cannon = 0;
	float time_acumulated = 0;
	float time_between_shots = 0.15;
};
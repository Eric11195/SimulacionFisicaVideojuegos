#pragma once

#include "ParticleSystem.hpp"
//#include "ParticleGeneratorsDescriptors.hpp"

class ShipCannon : public ParticleSystem {
public:
	ShipCannon();
	virtual void step(double dt) override;
	void start_fire();
	void stop_fire();
	void trigger_fire();
protected:
	bool active = false;
	virtual void shoot();
	virtual void step_fire(double dt);
	std::list<std::unique_ptr<GameObject>>::iterator cannon_it;
	float time_acumulated = 0;
	float time_between_shots = 0.15;
};
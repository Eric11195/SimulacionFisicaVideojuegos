#pragma once

#include "ParticleSystem.hpp"
//#include "ParticleGeneratorsDescriptors.hpp"

constexpr float time_between_shots = 0.5;

class ShipCannon : public ParticleSystem {
public:
	ShipCannon();
	virtual void step(double dt) override;
	void start_fire();
	void stop_fire();
protected:
	bool active = true;
	void shoot();
	std::list<std::unique_ptr<GameObject>>::iterator cannon_it;
	float time_acumulated = 0;
};
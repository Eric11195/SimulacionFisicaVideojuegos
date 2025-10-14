#include "ShipCannon.hpp"
#include "ParticleGeneratorsDescriptors.hpp"
#include <iostream>



ShipCannon::ShipCannon()
{
	for (int i = -1; i < 2; i = i + 2) {
		for (int j = -1; j < 2; j = j + 2) {
			GameObject* pg = new TriggeredParticleGenerator(x_wing_shoot_type);
			pg->translate_to({j*10.0f,i*10.0f,20});
			addChild(pg);
		}
	}
	cannon_it = child_objects.begin();
}

void ShipCannon::step(double dt)
{
	ParticleSystem::step(dt);

	if (!active) return;
	step_fire(dt);
}

void ShipCannon::start_fire()
{
	active = true;
	time_acumulated = time_between_shots;
}

void ShipCannon::stop_fire()
{
	active = false;
	//time_acumulated = time_between_shots;
}

void ShipCannon::trigger_fire()
{
	shoot();
}

void ShipCannon::shoot()
{
	GameObject* aux_ptr = (*cannon_it).get();
	auto casted_trigger = static_cast<TriggeredParticleGenerator*>(aux_ptr);
	casted_trigger->trigger();
}

void ShipCannon::step_fire(double dt)
{
	time_acumulated += dt;
	if (time_acumulated > time_between_shots) {
		time_acumulated -= time_between_shots;
		shoot();

		++cannon_it;
		if (cannon_it == child_objects.end()) cannon_it = child_objects.begin();
	}
}
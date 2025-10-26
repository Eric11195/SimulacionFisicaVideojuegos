#include "ShipCannon.hpp"
#include "ParticleGeneratorsDescriptors.hpp"
#include <iostream>
#include "MissileGenerator.hpp"
#include "BombGenerator.hpp"

ShipCannon::ShipCannon(Transform const& parent_tr)
	: ParticleSystem(parent_tr)
{
	for (int i = -1; i < 2; i = i + 2) {
		for (int j = -1; j < 2; j = j + 2) {
			GameObject* pg = new TriggeredParticleGenerator(x_wing_shoot_type);
			pg->translate_to({ j * 1.5f,i * 1.0f,0 });
			addChild(pg);
		}
	}
	cannon_it = child_objects.begin();

	GameObject* pg = new MissileGenerator();
	pg->translate_to({ 0,-1,0 });
	missile_cannon = addChild(pg);

	bomb_cannon = addChild(new BombGenerator(60,-90));
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

void ShipCannon::fire_missile()
{
	GameObject* aux_ptr = (*missile_cannon).get();
	auto casted_trigger = static_cast<MissileGenerator*>(aux_ptr);
	casted_trigger->trigger();
}

void ShipCannon::fire_bomb()
{
	//new Bomb();
	GameObject* aux_ptr = (*bomb_cannon).get();
	auto casted_trigger = static_cast<MissileGenerator*>(aux_ptr);
	casted_trigger->trigger();
}

/*
void ShipCannon::trigger_fire()
{
	fire_missile();
}
*/

void ShipCannon::normal_shoot()
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
		normal_shoot();

		++cannon_it;
		++n_cannon;
		if (n_cannon >= 4) {
			n_cannon = 0;
			cannon_it = child_objects.begin();
		}
	}
}
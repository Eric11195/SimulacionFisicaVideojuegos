#include "ShipCannon.hpp"
#include "ParticleGeneratorsDescriptors.hpp"
#include <iostream>
#include "MissileGenerator.hpp"
#include "BombGenerator.hpp"
#include "ShipRegularProjectileCannon.hpp"

ShipCannon::ShipCannon(Transform const& parent_tr)
	: ParticleSystem(parent_tr)
{
	GameObject* pg = new ShipRegularProjectileCannon();
	addChild(pg);
	normal_cannon_idx = 0;

	missile_cannon = addChild(new MissileGenerator());

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
std::vector<physx::PxVec3> cannon_pos = {
	{-1.5,1,0},
	{1.5, 1,0},
	{-1.5,-1,0},
	{1.5,-1,0}
};

void ShipCannon::normal_shoot()
{
	GameObject* aux_ptr = (*(child_objects.begin())).get();
	Transform tr = global_transform;
	tr.p += global_transform.q.rotate(cannon_pos[normal_cannon_idx]);
	aux_ptr->setTransform(tr);
	auto casted_trigger = static_cast<TriggeredParticleGenerator*>(aux_ptr);
	casted_trigger->trigger();

	++normal_cannon_idx;
	if (normal_cannon_idx >= cannon_pos.size()) normal_cannon_idx = 0;
}

void ShipCannon::step_fire(double dt)
{
	time_acumulated += dt;
	if (time_acumulated > time_between_shots) {
		time_acumulated -= time_between_shots;
		normal_shoot();
	}
}
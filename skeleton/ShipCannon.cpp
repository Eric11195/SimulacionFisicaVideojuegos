#include "ShipCannon.hpp"
#include "ParticleGeneratorsDescriptors.hpp"
#include <iostream>
#include "MissileGenerator.hpp"
#include "BombGenerator.hpp"
#include "ShipRegularProjectileCannon.hpp"
#include "SpringJoinedProjectileLauncher.hpp"

ShipCannon::ShipCannon(physx::PxScene* s, Transform const& parent_tr)
	: ParticleSystem(s, parent_tr)
{
	GameObject* pg = new ShipRegularProjectileCannon(s);
	addChild(pg);
	normal_cannon_idx = 0;

	missile_cannon = addChild(new MissileGenerator(s));

	spring_proj_cannon = addChild(new SpringJoinedProjectileLauncher(s));

	//bomb_cannon = addChild(new BombGenerator(s,60,50,1));
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
	Transform tr = global_transform;
	tr.p += global_transform.q.rotate({0,-1,2});
	aux_ptr->setTransform(tr);
	auto casted_trigger = static_cast<MissileGenerator*>(aux_ptr);

	casted_trigger->trigger(&currentInertia);
}

void ShipCannon::fire_spring()
{
	GameObject* aux_ptr = (*spring_proj_cannon).get();
	Transform tr = global_transform;
	tr.p += global_transform.q.rotate({ 0,0,2 });
	aux_ptr->setTransform(tr);
	auto casted_trigger = static_cast<SpringJoinedProjectileLauncher*>(aux_ptr);

	casted_trigger->trigger(&currentInertia);
}


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
	casted_trigger->trigger(&currentInertia);

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
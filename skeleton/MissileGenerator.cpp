#include "MissileGenerator.hpp"
#include "ParticleGeneratorsDescriptors.hpp"
#include <iostream>
#include "ForceGenerator.hpp"

/*
MissileCannon::MissileCannon() {
	GameObject* pg = new Missile();
	pg->translate_to({ 0,-10.0f,20 });
	addChild(pg);
}
*/

MissileGenerator::MissileGenerator(physx::PxScene* s)
	:TriggeredParticleGenerator(s,missile)
{
}

void* MissileGenerator::set_up_particle(Particle::config& p)
{
	auto part = new Missile(my_scene, Particle::config{ p.spho_config,p.time_till_death });
	for (auto& f : force_names) {
		part->add_force_to_myself(f);
	}
	for (auto f : force_ptr) {
		part->add_force_to_myself(f);
	}
	return part;
}

Missile::Missile(physx::PxScene* s, Particle::config& c)
	:Projectile(s, c, 3000.0f, c.spho_config.so_config.go_config.initial_speed_magnitude)
{
	addChild(
		new ForceAffected_ParticleGenerator(s, missile_particle_system, {},
			{ new Wind_ForceGenerator(s, {0,0,-1}, 100) })
	);
}

void Missile::step(double dt)
{
	Projectile::step(dt);
	/*
	integrate(dt);
	cout << 

	for (auto& child : child_objects) {
		child->setTransform(global_transform);
		child->step(dt);
	}
	*/
}

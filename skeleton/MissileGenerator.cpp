#include "MissileGenerator.hpp"
#include "ParticleGeneratorsDescriptors.hpp"
#include <iostream>

/*
MissileCannon::MissileCannon() {
	GameObject* pg = new Missile();
	pg->translate_to({ 0,-10.0f,20 });
	addChild(pg);
}
*/

MissileGenerator::MissileGenerator()
	:TriggeredParticleGenerator(missile, { "gravity" })
{
}

Particle* MissileGenerator::set_up_particle(Particle::config& p)
{
	auto part = new Missile(p);
	for (auto& f : force_names) {
		part->add_force_to_myself(f);
	}
	for (auto f : force_ptr) {
		part->add_force_to_myself(f);
	}
	return part;
}

Missile::Missile(Particle::config& c) 
	:Particle(c)
{
	addChild(new ParticleGenerator(missile_particle_system));
}

void Missile::step(double dt)
{
	integrate(dt);

	for (auto& child : child_objects) {
		child->setTransform(global_transform);
		child->step(dt);
	}
}

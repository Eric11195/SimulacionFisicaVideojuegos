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
	auto part = TriggeredParticleGenerator::set_up_particle(p);
	part->addChild(new ParticleGenerator(missile_particle_system));
	return part;
}
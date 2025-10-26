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

void MissileGenerator::set_up_particle(Particle* p)
{
	TriggeredParticleGenerator::set_up_particle(p);
	auto c = new ParticleGenerator(missile_particle_system);
	p->addChild(c);
}

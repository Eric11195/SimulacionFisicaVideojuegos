#include "Missile.hpp"
#include "ParticleGeneratorsDescriptors.hpp"


MissileCannon::MissileCannon() {
	GameObject* pg = new Missile();
	pg->translate_to({ 0,-10.0f,20 });
	addChild(pg);
}

Missile::Missile()
	:TriggeredParticleGenerator(missile)
{
}

void Missile::set_up_particle(Particle* p)
{
	p->addChild(new ParticleGenerator(missile_particle_system));
	addChild(p);
}

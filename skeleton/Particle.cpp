#include "Particle.hpp"

Particle::Particle(PxScene* s, config& c)
	:SphereObject(s, c.spho_config), time_till_death(c.time_till_death)
{
}

void Particle::step(double dt)
{
	time_till_death -= dt;
	SphereObject::step(dt);
}
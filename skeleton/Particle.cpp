#include "Particle.hpp"

Particle::Particle(config& c)
	:SphereObject(c.spho_config), time_till_death(c.time_till_death)
{
}
/*
Particle::Particle(config_particle_in_system& c, PxShape* shape)
	:SphereObject(c.scene_obj_config, shape)
{
}
*/
void Particle::step(double dt)
{
	time_till_death -= dt;
	SphereObject::step(dt);
}
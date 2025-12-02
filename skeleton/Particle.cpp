#include "Particle.hpp"
#include <iostream>

Particle::Particle(PxScene* s, config& c)
	:SphereObject(s, c.spho_config)
{
	my_class_id = particle;
	time_till_death = c.time_till_death;
}

void Particle::step(double dt)
{
	time_till_death -= dt;
	SphereObject::step(dt);
}

RigidParticle::RigidParticle(physx::PxScene* s, Particle::config& c) 
	:Rigid_SphereObject(s,c.spho_config)
{
	my_class_id = rb_particle;
	//std::cout << "till death: " << c.time_till_death<<'\n';
	time_till_death = c.time_till_death;
}
/*
RigidParticle::~RigidParticle()
{
	rb->
}
*/

void RigidParticle::step(double dt)
{
	time_till_death -= dt;
	Rigid_SphereObject::step(dt);
}

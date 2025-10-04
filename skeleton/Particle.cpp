#include "Particle.hpp"

Particle::Particle(config c)
	:vel(c.initial_dir.normalize() * c.initial_speed), accel(My_Vector3::zero()),
	SphereObject(c.radius, c.pos), time_till_death(c.time_till_death)
{
}

void Particle::init()
{

}

void Particle::step(double dt)
{
	time_till_death -= dt;
	integrate(dt);
}


void Particle::cleanup()
{
	SphereObject::cleanup();
}

void Particle::set_accel(My_Vector3 new_accel)
{
	accel = new_accel;
}

void Particle::add_accel(My_Vector3 add_accel)
{
	accel += add_accel;
}

#ifdef DAMPING
void Particle::set_dumping(float f)
{
	damping_mult = f;
}
#endif

void Particle::integrate(double dt)
{

#if defined EULER_SEMI_EXPLICIT_INTEGRATION
	vel += accel * dt;
	//tr.p += dt * vel.turn();
	translate(dt * vel.turn());
#elif defined EULER_INTEGRATION
	tr.p += dt * vel.turn();
	vel += accel*dt;
#endif

#ifdef DAMPING
	vel *= pow(damping_mult, dt);
#endif

}

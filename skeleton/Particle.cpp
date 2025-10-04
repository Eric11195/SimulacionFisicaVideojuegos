#include "Particle.hpp"

Particle::Particle(My_Vector3 _pos, My_Vector3 _vel, float _radius)
	: vel(_vel), accel(My_Vector3::zero()),
	SphereObject(_radius, _pos) {
}

void Particle::init()
{

}

void Particle::step(double dt)
{
	integrate(dt);
	//local_transform.transform();
	//tr.transform(vel.turn());
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

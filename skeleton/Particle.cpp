#include "Particle.hpp"

Particle::Particle(My_Vector3 _pos, My_Vector3 _vel)
	: vel(_vel), accel(My_Vector3::zero()),
	SphereObject(PARTICLE_RADIUS, _pos) {

}

void Particle::init()
{

}

void Particle::step(double dt)
{
	integrate(dt);
	tr.transform(vel.turn());
}

void Particle::cleanup()
{
	SphereObject::cleanup();
}

void Particle::change_accel(My_Vector3 new_accel)
{
	accel = new_accel;
}

void Particle::integrate(double dt)
{

#if defined EULER_SEMI_EXPLICIT_INTEGRATION
	vel += accel * dt;
	tr.p += dt * vel.turn();
#elif defined EULER_INTEGRATION
	tr.p += dt * vel.turn();
	vel += accel*dt;
#endif

#ifdef DAMPING
	vel *= pow(DAMPING_MULT, dt);
#endif

}

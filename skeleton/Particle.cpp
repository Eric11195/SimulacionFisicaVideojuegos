#include "Particle.hpp"

Particle::Particle(My_Vector3 _pos, My_Vector3 _vel)
	: vel(_vel),
	SphereObject(PARTICLE_RADIUS, _pos) {

}

Particle::~Particle()
{
}

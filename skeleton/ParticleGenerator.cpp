#include "ParticleGenerator.hpp"

ParticleGenerator::ParticleGenerator(config c)
	:GameObject(Transform(c.origin.turn()))
{
}

void ParticleGenerator::step(double dt)
{
	for (auto particle : particle_list) {
		particle->step(dt);
	}
}

void ParticleGenerator::cleanup()
{
	for (auto particle : particle_list)
		particle->cleanup();
	GameObject::cleanup();
}

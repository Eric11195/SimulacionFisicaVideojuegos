#include "ParticleGenerator.hpp"
#include "Particle.hpp"

ParticleGenerator::ParticleGenerator(config c)
	:GameObject(Transform(c.origin.turn())), particles_generated_per_step(c.particle_generated_per_step),
	avrg_speed(c.average_speed)
{
}

void ParticleGenerator::step(double dt)
{
	auto it = particle_list.begin();
	while (it != particle_list.end()) {
		auto casted_particle = dynamic_cast<Particle*>(*it);
		if (!casted_particle->alive()) {
			//MAY EXPLODE
			(*it)->cleanup();
			it = particle_list.erase(it);
			continue;
		}
		(*it)->step(dt);
		++it;
	}
}

void ParticleGenerator::cleanup()
{
	for (auto particle : particle_list)
		particle->cleanup();
	GameObject::cleanup();
}

void ParticleGenerator::generate_particles()
{
}

//-------------------------------------------------------------------------------------------------------
TriggeredParticleGenerator::TriggeredParticleGenerator(ParticleGenerator::config c)
	:ParticleGenerator(c)
{
}

void TriggeredParticleGenerator::Trigger()
{
	generate_particles();
}

ToggleParticleGenerator::ToggleParticleGenerator(ParticleGenerator::config c, bool initial_state)
	:ParticleGenerator(c), state(initial_state)
{
}

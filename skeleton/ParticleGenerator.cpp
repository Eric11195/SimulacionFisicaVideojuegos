#include "ParticleGenerator.hpp"
#include "Particle.hpp"

ParticleGenerator::ParticleGenerator(config c)
	:GlobalCoords_CompositeGameObject(c.particle_config.spho_config.
		so_config.go_config), 
	particle_generated_per_second(c.particle_generated_per_second),
	avrg_speed(c.particle_config.spho_config.
		so_config.go_config.initial_speed_magnitude), 
	p_config(c.particle_config), my_particle_lambdas(c.particle_lambdas) {}

void ParticleGenerator::step(double dt)
{
	generate_particles(dt);

	auto it = child_objects.begin();
	while (it != child_objects.end()) {
		
		auto casted_particle = static_cast<Particle*>(*it);
		if (!casted_particle->alive()) {
			//MAY EXPLODE
			(*it)->cleanup();
			//delete (*it);
			it = child_objects.erase(it);
			continue;
		}
		(*it)->step(dt);
		(*it)->update_position(PhysicLib::NEUTRAL_TRANSFORM);
		++it;
	}
}

void ParticleGenerator::cleanup()
{
	for (auto particle : child_objects)
		particle->cleanup();
	GameObject::cleanup();
}

void ParticleGenerator::generate_particles(double dt)
{
	float particles_this_frame = particles_per_second_accumulator + particle_generated_per_second * dt;
	int particles_generated_in_current_frame = floor(particles_this_frame);
	particles_per_second_accumulator = particles_this_frame - particles_generated_in_current_frame;
	auto& p_config_short = p_config.spho_config.so_config.go_config;
	for (int i = 0; i < particles_generated_in_current_frame; ++i) {
		p_config_short.pos = My_Vector3::unturn(global_transform.p) + my_particle_lambdas.pos();
		p_config_short.initial_accel_magnitude = -10;
		p_config_short.initial_accel_dir = { 0,-1,0 };
		p_config_short.initial_speed_magnitude = avrg_speed + my_particle_lambdas.vel();
		p_config_short.initial_speed_dir = My_Vector3{ 0,1,0 } + my_particle_lambdas.dir();
		auto new_particle = new Particle(p_config);
		addChild(new_particle);
	}

}

//-------------------------------------------------------------------------------------------------------
TriggeredParticleGenerator::TriggeredParticleGenerator(ParticleGenerator::config c)
	:ParticleGenerator(c)
{
}

void TriggeredParticleGenerator::Trigger()
{
	generate_particles(0);
}

ToggleParticleGenerator::ToggleParticleGenerator(ParticleGenerator::config c, bool initial_state)
	:ParticleGenerator(c), state(initial_state)
{
}

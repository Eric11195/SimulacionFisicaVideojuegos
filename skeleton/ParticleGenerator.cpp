#include "ParticleGenerator.hpp"
#include <iostream>
#include "ForceGenerator.hpp"
//#include "Particle.hpp"

//-------------------------------------------------------------------------------------------------------
TriggeredParticleGenerator::TriggeredParticleGenerator(ParticleGenerator::config& c, std::initializer_list<std::string> forces_names, std::initializer_list<ForceGenerator*> forces_ptr)
	:ForceAffected_ParticleGenerator(c,forces_names, forces_ptr)
{
}


void TriggeredParticleGenerator::trigger()
{
	generate_particles(1);
}


void TriggeredParticleGenerator::step(double dt)
{

	//Missing generate particles
	auto it = child_objects.begin();
	while (it != child_objects.end()) {

		GameObject* aux_ptr = (*it).get();
		if (!my_particle_lambdas.inside_area_of_interest((*it)->get_pos(), this->get_pos()))
		{
			it = child_objects.erase(it);
			continue;
		}
		auto casted_particle = static_cast<Particle*>(aux_ptr);
		if (!casted_particle->alive()) {
			it = child_objects.erase(it);
			continue;
		}
		//(*it)->step(dt);
		
		++it;
	}
	GameObject::step(dt);
}


ToggleParticleGenerator::ToggleParticleGenerator(ParticleGenerator::config& c, bool initial_state)
	:ParticleGenerator(c), state(initial_state)
{
}

//-------------------------------------------------------------------------------------------------------

ForceAffected_ParticleGenerator::ForceAffected_ParticleGenerator(ParticleGenerator::config& c, std::initializer_list<std::string> forces, std::initializer_list<ForceGenerator*> forces_ptr)
	:ParticleGenerator(c), force_names(forces), force_ptr(forces_ptr) {}

Particle* ForceAffected_ParticleGenerator::set_up_particle(Particle::config& p)
{
	auto particle = ParticleGenerator::set_up_particle(p);
	for (auto& f : force_names) {
		particle->add_force_to_myself(f);
	}
	for (auto f : force_ptr) {
		particle->add_force_to_myself(f);
	}
	return  particle;
}

void ForceAffected_ParticleGenerator::step(double dt)
{
	ParticleGenerator::step(dt);
	for (auto f : force_ptr) {
		f->setTransform(global_transform);
	}
}

ParticleGenerator::ParticleGenerator(config& c)
	:
	const_p_config(c.particle_config.spho_config.so_config.go_config),
	particle_generated_per_second(c.particle_generated_per_second),
	particle_start_pos(c.particle_config.spho_config.so_config.go_config.pos),
	avrg_speed(c.particle_config.spho_config.
		so_config.go_config.initial_speed_magnitude),
	avrg_mass(c.particle_config.spho_config.so_config.go_config.mass),
	avrg_lifetime(c.particle_config.time_till_death),
	avrg_color(c.particle_config.spho_config.so_config.color),
	avrg_size(c.particle_config.spho_config.radius),
	p_config(c.particle_config), my_particle_lambdas(c.particle_lambdas) {
}

void ParticleGenerator::step(double dt)
{
	generate_particles(dt);
	//GameObject::step(dt);

	auto it = child_objects.begin();
	while (it != child_objects.end()) {
		if (!my_particle_lambdas.inside_area_of_interest((*it)->get_pos(), this->get_pos()))
		{
			it = child_objects.erase(it);
			continue;
		}

		GameObject* aux_ptr = (*it).get();
		auto casted_particle = static_cast<Particle*>(aux_ptr);
		if (!casted_particle->alive()) {
			it = child_objects.erase(it);
			continue;
		}
		(*it)->step(dt);
		//(*it)->update_position(PhysicLib::NEUTRAL_TRANSFORM);

		++it;
	}
}

void ParticleGenerator::generate_particles(double dt)
{
	float particles_this_frame = particles_per_second_accumulator + particle_generated_per_second * dt;
	int particles_generated_in_current_frame = floor(particles_this_frame);
	particles_per_second_accumulator = particles_this_frame - particles_generated_in_current_frame;
	auto& new_p_config_short = p_config.spho_config.so_config.go_config;
	for (int i = 0; i < particles_generated_in_current_frame; ++i) {
		p_config.spho_config.radius = avrg_size + my_particle_lambdas.size();
		if (p_config.spho_config.radius <= 0) continue;
		new_p_config_short.initial_rotation = global_transform.q;
		new_p_config_short.pos = global_transform.p + global_transform.q.rotate(particle_start_pos + my_particle_lambdas.pos());
		//new_p_config_short.initial_accel_magnitude; //= 30;
		//new_p_config_short.initial_accel_dir = const_p_config.initial_accel_dir;
		new_p_config_short.initial_speed_magnitude = avrg_speed + my_particle_lambdas.vel();
		new_p_config_short.initial_speed_dir = global_transform.q.rotate(const_p_config.initial_speed_dir + my_particle_lambdas.dir());
		new_p_config_short.mass = avrg_mass + my_particle_lambdas.mass();
		p_config.time_till_death = avrg_lifetime + my_particle_lambdas.lifetime();
		p_config.spho_config.so_config.color = avrg_color + my_particle_lambdas.color();
		addChild(set_up_particle(p_config));
	}
}

Particle* ParticleGenerator::set_up_particle(Particle::config& p)
{
	return new Particle(p);
}

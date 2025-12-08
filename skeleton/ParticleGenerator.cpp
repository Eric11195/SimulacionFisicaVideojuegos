#include "ParticleGenerator.hpp"
#include <iostream>
#include "ForceGenerator.hpp"
#include <cassert>

//#include "Particle.hpp"

//-------------------------------------------------------------------------------------------------------
TriggeredParticleGenerator::TriggeredParticleGenerator(physx::PxScene* s, ParticleGenerator::config& c, std::initializer_list<std::string> forces_names, std::initializer_list<ForceGenerator*> forces_ptr)
	:ForceAffected_ParticleGenerator(s,c,forces_names, forces_ptr)
{
}


void TriggeredParticleGenerator::trigger(void* v)
{
	generate_particles(1, v);
}


void TriggeredParticleGenerator::step(double dt)
{
	GameObject::step(dt);
}


//-------------------------------------------------------------------------------------------------------

ForceAffected_ParticleGenerator::ForceAffected_ParticleGenerator(physx::PxScene* s, ParticleGenerator::config& c, std::initializer_list<std::string> forces, std::initializer_list<ForceGenerator*> forces_ptr)
	:ParticleGenerator(s,c), force_names(forces), force_ptr(forces_ptr) {}

GameObject* ForceAffected_ParticleGenerator::set_up_particle(Particle::config& p, void* v)
{
	auto particle = static_cast<GameObject*>(ParticleGenerator::set_up_particle(p, v));
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
}

ParticleGenerator::ParticleGenerator(physx::PxScene* s, config& c)
	:
	const_p_config(c.particle_config.spho_config.so_config.go_config),
	particle_generated_per_second(c.particle_generated_per_second),
	avrg_lifetime(c.particle_config.time_till_death),
	avrg_color(c.particle_config.spho_config.so_config.color),
	avrg_size(c.particle_config.spho_config.radius),
	p_config(c.particle_config), my_particle_lambdas(c.particle_lambdas), GameObject(s){
}

void ParticleGenerator::step(double dt)
{
	generate_particles(dt, nullptr);
	auto it = child_objects.begin();
	while (it != child_objects.end()) {
		if (!my_particle_lambdas.inside_area_of_interest((*it)->get_pos(), this->get_pos())) {
			//La partícula esta fuera del area de interes y debería ser borrada.
			it = child_objects.erase(it);
		}
		else ++it;
	}
	GameObject::step(dt);
}

void ParticleGenerator::generate_particles(double dt, void* v)
{
	float particles_this_frame = particles_per_second_accumulator + particle_generated_per_second * dt;
	int particles_generated_in_current_frame = floor(particles_this_frame);
	particles_per_second_accumulator = particles_this_frame - particles_generated_in_current_frame;
	auto& new_p_config_short = p_config.spho_config.so_config.go_config;
	for (int i = 0; i < particles_generated_in_current_frame; ++i) {
		p_config.spho_config.radius = avrg_size + my_particle_lambdas.size();
		if (p_config.spho_config.radius <= 0) continue;
		new_p_config_short.initial_rotation = global_transform.q;
		new_p_config_short.pos = global_transform.p + global_transform.q.rotate(const_p_config.pos + my_particle_lambdas.pos());
		//new_p_config_short.initial_accel_magnitude; //= 30;
		//new_p_config_short.initial_accel_dir = const_p_config.initial_accel_dir;
		new_p_config_short.initial_speed_magnitude = const_p_config.initial_speed_magnitude + my_particle_lambdas.vel();
		new_p_config_short.initial_speed_dir = global_transform.q.rotate(const_p_config.initial_speed_dir + my_particle_lambdas.dir());
		new_p_config_short.mass = const_p_config.mass + my_particle_lambdas.mass();
		p_config.time_till_death = avrg_lifetime + my_particle_lambdas.lifetime();
		p_config.spho_config.so_config.color = avrg_color + my_particle_lambdas.color();
		addChild(static_cast<GameObject*>(set_up_particle(p_config, v)));
	}
}

GameObject* ParticleGenerator::set_up_particle(Particle::config& p, void* v)
{
	return new Particle(my_scene, p);
}

ToggleParticleGenerator::ToggleParticleGenerator(physx::PxScene* s, ParticleGenerator::config& c, std::initializer_list<std::string> forces, std::initializer_list<ForceGenerator*> forces_ptr)
	:ForceAffected_ParticleGenerator(s,c,forces,forces_ptr), active(true)
{
}

void ToggleParticleGenerator::set_toggle(bool state)
{
	active = state;
}

void ToggleParticleGenerator::step(double dt)
{
	if (active) generate_particles(dt, nullptr);
	auto it = child_objects.begin();
	while (it != child_objects.end()) {
		if (!my_particle_lambdas.inside_area_of_interest((*it)->get_pos(), this->get_pos())) {
			//La partícula esta fuera del area de interes y debería ser borrada.
			it = child_objects.erase(it);
		}
		else ++it;
	}
	GameObject::step(dt);
}

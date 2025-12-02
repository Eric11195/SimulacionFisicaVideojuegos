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

	//Missing generate particles
	auto it = child_objects.begin();
	while (it != child_objects.end()) {
		auto ptr = (*it).get();
		switch (ptr->my_class_id) {
		case uninteresting:
			assert(false);
			break;
		case particle:
		{
			Particle* casted_particle = static_cast<Particle*>((*it).get());
			if (!casted_particle->alive()) {
				it = child_objects.erase(it);
				continue;
			}
			else if (!my_particle_lambdas.inside_area_of_interest((*it)->get_pos(), this->get_pos()))
			{
				it = child_objects.erase(it);
				continue;
			}
			break;
		}
		case rb_particle: {
			RigidParticle* casted_particle = static_cast<RigidParticle*>((*it).get());
			if (!casted_particle->alive()) {
				it = child_objects.erase(it);
				continue;
			}
			else if (!my_particle_lambdas.inside_area_of_interest((*it)->get_pos(), this->get_pos()))
			{
				it = child_objects.erase(it);
				continue;
			}
			break;
		}
		}
		++it;

	}
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
	for (auto f : force_ptr) {
		f->setTransform(global_transform);
	}
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
	//GameObject::step(dt);

	auto it = child_objects.begin();
	while (it != child_objects.end()) {

		void* ptr = (*it).get();
		InterfaceParticle* casted_particle = static_cast<InterfaceParticle*>(ptr);
		if (!casted_particle->alive()) {
			it = child_objects.erase(it);
			continue;
		}else		if (!my_particle_lambdas.inside_area_of_interest((*it)->get_pos(), this->get_pos()))
		{
			it = child_objects.erase(it);
			continue;
		}
		else {
			(*it)->step(dt);
			++it;
		}
	}
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
	//GameObject::step(dt);
	if(active) generate_particles(dt, nullptr);
	//GameObject::step(dt);

	auto it = child_objects.begin();
	while (it != child_objects.end()) {

		void* ptr = (*it).get();
		InterfaceParticle* casted_particle = static_cast<InterfaceParticle*>(ptr);
		if (!casted_particle->alive()) {
			it = child_objects.erase(it);
			continue;
		}else if (!my_particle_lambdas.inside_area_of_interest((*it)->get_pos(), this->get_pos()))
		{
			it = child_objects.erase(it);
			continue;
		}
		else {
			(*it)->step(dt);
			//(*it)->update_position(PhysicLib::NEUTRAL_TRANSFORM);

			++it;
		}
	}
	/*
	auto it = child_objects.begin();
	while (it != child_objects.end()) {
		GameObject* aux_ptr = (*it).get();
		auto casted_particle = (InterfaceParticle*)(aux_ptr);
		assert(casted_particle);
		if (!casted_particle->alive()) {
			it = child_objects.erase(it);
			continue;
		}else if (!my_particle_lambdas.inside_area_of_interest((*it)->get_pos(), this->get_pos()))
		{
			it = child_objects.erase(it);
			continue;
		}
		else {
			(*it)->step(dt);
			++it;
		}
	}

	if (active) {
		generate_particles(dt, nullptr);
	}
	*/
}

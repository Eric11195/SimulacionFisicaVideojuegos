#include "ParticleGenerator.hpp"
#include <iostream>
//#include "Particle.hpp"

ParticleGenerator::ParticleGenerator(config& c)
	:GlobalCoords_CompositeGameObject(c.go_config),
	const_p_config(c.particle_config.spho_config.so_config.go_config),
	particle_generated_per_second(c.particle_generated_per_second),
	avrg_speed(c.particle_config.spho_config.
		so_config.go_config.initial_speed_magnitude),
	avrg_lifetime(c.particle_config.time_till_death),
	avrg_color(c.particle_config.spho_config.so_config.color),
	avrg_size(c.particle_config.spho_config.radius),
	p_config(c.particle_config), my_particle_lambdas(c.particle_lambdas) {}

void ParticleGenerator::step(double dt)
{
	generate_particles(dt);
	//GameObject::step(dt);
	integrate(dt);

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
		new_p_config_short.pos = global_transform.p + my_particle_lambdas.pos();
		new_p_config_short.initial_rotation = global_transform.q;
		new_p_config_short.initial_accel_magnitude; //= 30;
		new_p_config_short.initial_accel_dir = const_p_config.initial_accel_dir;
		new_p_config_short.initial_speed_magnitude = avrg_speed + my_particle_lambdas.vel();
		new_p_config_short.initial_speed_dir = const_p_config.initial_speed_dir + my_particle_lambdas.dir();
		p_config.time_till_death = avrg_lifetime + my_particle_lambdas.lifetime();
		p_config.spho_config.so_config.color = avrg_color + my_particle_lambdas.color();
		auto new_particle = new Particle(p_config);
		set_up_particle(new_particle);

		/*
		auto& u = global_transform.p;
		auto& q = global_transform.q;
		std::cout << "Parent: " << u.x << " " << u.y << " " << u.z << " --- " << q.x << " " << q.y << " " << q.z << " " << q.w << '\n';

		auto& u2 = new_particle->global_transform.p;
		auto& q2 = new_particle->global_transform.q;
		std::cout << "Child:  " << u2.x << " " << u2.y << " " << u2.z << " --- " << q2.x << " " << q2.y << " " << q2.z << " " << q2.w << '\n';
		*/
		/*
		auto& u3 = new_particle->child_objects.front()->global_transform.p;
		auto& q3 = new_particle->child_objects.front()->global_transform.q;
		std::cout << "Gen:    " << u3.x << " " << u3.y << " " << u3.z << " --- " << q3.x << " " << q3.y << " " << q3.z << " " << q3.w << '\n';
		*/
	}

}

void ParticleGenerator::set_up_particle(Particle* p)
{
	addChild(p);
}

//-------------------------------------------------------------------------------------------------------
TriggeredParticleGenerator::TriggeredParticleGenerator(ParticleGenerator::config c)
	:ParticleGenerator(c)
{
}

void TriggeredParticleGenerator::trigger()
{
	generate_particles(1);
}

void TriggeredParticleGenerator::step(double dt)
{
	integrate(dt);
	//GameObject::step(dt);

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

ToggleParticleGenerator::ToggleParticleGenerator(ParticleGenerator::config c, bool initial_state)
	:ParticleGenerator(c), state(initial_state)
{
}

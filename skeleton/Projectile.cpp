#include "Projectile.hpp"
#include <iostream>
#include <cassert>
#include "ForceGenerator.hpp"

Projectile::Projectile(projectile_config& c, float real_speed, float simulated_speed)
	:Particle(c.particle_config)
{
	set_velocity_magnitude(simulated_speed);
	gravity_multiplier = get_gravity_proportion(real_speed, simulated_speed);
	mass = Mass(get_s_mass(c.particle_config.spho_config.so_config.go_config.mass.mass, real_speed, simulated_speed));
	//ENSURE THIS IS THE FIRST FORCE APPLIED TO THIS OBJECT
	assert(forces_applied_to_this_obj.size() == 0);
	add_force_to_myself("gravity");
}

float Projectile::get_s_gravity(float real_speed, float sim_speed)
{
	return 9.8f * get_gravity_proportion(real_speed,sim_speed);
}

float Projectile::get_gravity_proportion(float real_speed, float sim_speed)
{
	return (sim_speed * sim_speed) / (real_speed * real_speed);
}

float Projectile::get_s_mass(float mass, float real_speed, float sim_speed)
{
	float vel_proportion = (real_speed * real_speed) / (sim_speed * sim_speed);
	return mass * vel_proportion;
}

void Projectile::integrate(double dt)
{
#if defined EULER_SEMI_EXPLICIT_INTEGRATION
	//In theory this does not exist
	physx::PxVec3 accel = { 0,0,0 };
	auto force_it = forces_applied_to_this_obj.begin();
	//Gravity
	accel += (*force_it)->apply_force(*this) * gravity_multiplier;
	++force_it;
	while (force_it != forces_applied_to_this_obj.end()) {
		auto new_accel = (*force_it)->apply_force(*this);
		accel += new_accel;
	}

	//F = m * a, así que si solo le añado todas las fuerzas a accel. Antes de poder añadirselo a la velocidad tengo que dividirlo por la masa (o multiplicarlo por la masa inversa)
	accel *= mass.inv_mass;
	//std::cout << vel.x << " " << vel.y << " " << vel.z << '\n';
	vel += accel * dt;
	translate(dt * vel);
#elif defined EULER_INTEGRATION
	tr.p += dt * vel.turn();
	vel += accel * dt;
#endif

#ifdef DAMPING
	vel *= pow(damping_mult, dt);
#endif
}
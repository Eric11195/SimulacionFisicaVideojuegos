#include "Projectile.hpp"
#include <iostream>

Projectile::Projectile(projectile_config& c)
	: Particle(c.particle_config)//,
	//m_real(c.particle_config.spho_config.so_config.go_config.mass.mass), s_real(c.s_real), g_real(9.8f),
	//s_simulated(c.particle_config.spho_config.so_config.go_config.initial_speed_magnitude)
{
//	set_accel({0,-g_simulated, 0});
}

float Projectile::get_s_gravity(float real_speed, float sim_speed)
{
	float vel_proportion = (sim_speed * sim_speed)/ (real_speed * real_speed);
	return 9.8f * vel_proportion;
}

float Projectile::get_s_mass(float mass, float real_speed, float sim_speed)
{
	float vel_proportion = (real_speed * real_speed) / (sim_speed * sim_speed);
	return mass * vel_proportion;
}
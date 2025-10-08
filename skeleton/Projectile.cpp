#include "Projectile.hpp"
#include <iostream>

void Projectile::step(double dt)
{
	//std::cout << "POS: " << tr.p.x << ", " << tr.p.y << ", " << tr.p.z <<"  "<<
		//"VEL: "<< tr.p.x << ", " << tr.p.y << ", " << tr.p.z<<'\n';
	Particle::step(dt);
}

Projectile::Projectile(projectile_config& c)
	: Particle(c.particle_config),
	m_real(c.m_real), s_real(c.s_real), g_real(c.m_gravity),
	s_simulated(c.particle_config.spho_config.so_config.go_config.initial_speed_magnitude)
{
	update_s_mass();
	update_s_gravity();
	set_accel({0,-g_simulated, 0});
}

void Projectile::update_s_gravity()
{
	float vel_proportion = (s_simulated * s_simulated)/(s_real * s_real);
	g_simulated = g_real * vel_proportion;
}

void Projectile::update_s_mass()
{

	float vel_proportion = (s_simulated*s_simulated)/ (s_real * s_real);
	m_simulated = m_real * vel_proportion;
}

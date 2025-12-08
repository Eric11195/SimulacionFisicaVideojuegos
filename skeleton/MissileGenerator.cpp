#include "MissileGenerator.hpp"
#include "ParticleGeneratorsDescriptors.hpp"
#include <iostream>
#include "ForceGenerator.hpp"

MissileGenerator::MissileGenerator(physx::PxScene* s)
	:TriggeredParticleGenerator(s,missile)
{
}

GameObject* MissileGenerator::set_up_particle(Particle::config& p, void* v)
{
	auto inertial_speed = *static_cast<physx::PxVec3*>(v);
	auto c = Particle::config{ p.spho_config,p.time_till_death };
	auto my_init_speed = c.spho_config.so_config.go_config.initial_speed_dir * c.spho_config.so_config.go_config.initial_speed_magnitude;
	my_init_speed = my_init_speed + inertial_speed;
	//c.spho_config.so_config.go_config.pos = c.spho_config.so_config.go_config.pos + my_init_speed;
	c.spho_config.so_config.go_config.initial_speed_magnitude = my_init_speed.normalize();
	c.spho_config.so_config.go_config.initial_speed_dir = my_init_speed;

	auto part = new Missile(my_scene, c);
	part->my_team_id = player;
	for (auto& f : force_names) {
		part->add_force_to_myself(f);
	}
	for (auto f : force_ptr) {
		part->add_force_to_myself(f);
	}
	return part;
}

Missile::Missile(physx::PxScene* s, Particle::config& c)
	:Projectile(s, c, 3000.0f, c.spho_config.so_config.go_config.initial_speed_magnitude)
{
	addChild(
		new ForceAffected_ParticleGenerator(s, missile_particle_system, {},
			{ new Wind_ForceGenerator(s, {0,0,-1}, 1) })
	);
}

void Missile::step(double dt)
{
	Projectile::step(dt);
	for (auto& c : get_child_list()) {
		c->setTransform(global_transform);
	}
}

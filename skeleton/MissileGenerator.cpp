#include "MissileGenerator.hpp"
#include "ParticleGeneratorsDescriptors.hpp"
#include <iostream>
#include "ForceGenerator.hpp"

/*
MissileCannon::MissileCannon() {
	GameObject* pg = new Missile();
	pg->translate_to({ 0,-10.0f,20 });
	addChild(pg);
}
*/

MissileGenerator::MissileGenerator()
	:TriggeredParticleGenerator(missile)
{
}

Particle* MissileGenerator::set_up_particle(Particle::config& p)
{
	auto part = new Missile(Projectile::projectile_config{ p });
	for (auto& f : force_names) {
		part->add_force_to_myself(f);
	}
	for (auto f : force_ptr) {
		part->add_force_to_myself(f);
	}
	return part;
}

Missile::Missile(Projectile::projectile_config& c) 
	:Projectile(c, 3000.0f, c.particle_config.spho_config.so_config.go_config.initial_speed_magnitude)
{
	addChild(
		new ForceAffected_ParticleGenerator(missile_particle_system, {},
			{ new Wind_ForceGenerator({0,0,-1}, 5) })
	);
}

void Missile::step(double dt)
{
	integrate(dt);

	for (auto& child : child_objects) {
		child->setTransform(global_transform);
		child->step(dt);
	}
}

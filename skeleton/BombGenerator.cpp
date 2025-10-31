#include "BombGenerator.hpp"
#include "ForceGenerator.hpp"
#include "ParticleGeneratorsDescriptors.hpp"
#include <iostream>
#include <cmath>

BombGenerator::BombGenerator(float force_mag, float rad, float explosion_force_tao, std::initializer_list<std::string> forces)
	:TriggeredParticleGenerator(bomb), explosion_force_tao(explosion_force_tao),rad_squared(rad*rad), force_mag(force_mag)//, force_added_per_second(force_added_per_second)
{
}

Particle* BombGenerator::set_up_particle(Particle::config& p_config)
{
	auto p = TriggeredParticleGenerator::set_up_particle(p_config);
	double e = std::exp(1.0);
	auto force_raw_ptr = new Variable_ForceGenerator(force_mag,
		[=](float force_mag, float time, GameObject const& self, GameObject const& g) {
			auto v = g.get_global_tr().p - self.get_global_tr().p;
			auto dist_to_center_squared = v.magnitudeSquared();
			physx::PxVec3 force_applied = { 0,0,0 };
			if (dist_to_center_squared < rad_squared) {
				force_applied = force_mag / (dist_to_center_squared) *v * pow(e, -time/explosion_force_tao);
			}
			return force_applied;
		}
	);
	force_raw_ptr->setTransform(p->get_global_tr());

	//std::shared_ptr<ForceGenerator> force_ptr = std::shared_ptr<ForceGenerator>(force_raw_ptr);
	p->addChild(force_raw_ptr);
	auto generator = new TriggeredParticleGenerator(bomb_particle_generator, {}, {force_raw_ptr});// , { force_ptr });
	p->addChild(generator);
	generator->setTransform(p->get_global_tr());
	generator->trigger();
	return p;
}

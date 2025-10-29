#include "BombGenerator.hpp"
#include "ForceGenerator.hpp"
#include "ParticleGeneratorsDescriptors.hpp"
#include <iostream>

BombGenerator::BombGenerator(float force_mag, float force_added_per_second, std::initializer_list<std::string> forces)
	:TriggeredParticleGenerator(bomb), force_mag(force_mag), force_added_per_second(force_added_per_second)
{
}

Particle* BombGenerator::set_up_particle(Particle::config& p_config)
{
	auto p = TriggeredParticleGenerator::set_up_particle(p_config);
	
	auto force_raw_ptr = new Variable_ForceGenerator(force_mag,
		[](float force_mag, GameObject const& self, GameObject const& g) {
			const physx::PxVec3 force_applied = (g.get_global_tr().p - self.get_global_tr().p).getNormalized();
			return force_applied* force_mag;
		},
		//Captures force multiplier
		[=](double time, double dt, float& force_mag) {
			force_mag += force_added_per_second * dt;
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

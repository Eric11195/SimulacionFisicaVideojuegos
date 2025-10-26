#include "BombGenerator.hpp"
#include "ForceGenerator.hpp"
#include "ParticleGeneratorsDescriptors.hpp"

BombGenerator::BombGenerator(float force_mag, float force_reduction_coef, std::initializer_list<std::string> forces)
	:TriggeredParticleGenerator(bomb), force_mag(force_mag), force_reduction_coef(force_reduction_coef)
{
}

Particle* BombGenerator::set_up_particle(Particle::config& p_config)
{
	auto p = TriggeredParticleGenerator::set_up_particle(p_config);
	auto force_raw_ptr = new Variable_ForceGenerator(/*"ExplosionForce", */force_mag,
		[](float force_mag, GameObject const& self, GameObject const& g) {
			const physx::PxVec3 force_applied = (g.get_global_tr().p - self.get_global_tr().p).getNormalized();
			return physx::PxVec3(0, 0, 0);// force_applied* force_mag;
		},
		//Captures force multiplier
		[=](double time, double dt, float& force_mag) {
			force_mag -= force_mag*force_reduction_coef * dt;
		}
	);
	//std::shared_ptr<ForceGenerator> force_ptr = std::shared_ptr<ForceGenerator>(force_raw_ptr);
	p->addChild(force_raw_ptr);
	auto generator = new TriggeredParticleGenerator(bomb_particle_generator, {}, {force_raw_ptr});// , { force_ptr });
	addChild(generator);
	generator->trigger();
	return p;
}

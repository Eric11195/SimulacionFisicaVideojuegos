#include "BombGenerator.hpp"
#include "ForceGenerator.hpp"
#include "ParticleGeneratorsDescriptors.hpp"

BombGenerator::BombGenerator(ParticleGenerator::config& c, float force_mag, float force_reduction_coef, std::initializer_list<std::string> forces)
	:TriggeredParticleGenerator(c), force_mag(force_mag), force_reduction_coef(force_reduction_coef)
{
}

Particle* BombGenerator::set_up_particle(Particle::config& p_config)
{
	auto p = TriggeredParticleGenerator::set_up_particle(p_config);
	p->addChild(new Variable_ForceGenerator("ExplosionForce", force_mag,
		[](float force_mag, GameObject const& self, GameObject const& g) {
			const physx::PxVec3 force_applied = (g.get_global_tr().p - self.get_global_tr().p).getNormalized();
			return force_applied * force_mag;
		},
		//Captures force multiplier
		[=](double time, double dt, float& force_mag) {
			force_mag *= force_reduction_coef * dt;
		}
	));
	auto generator = new TriggeredParticleGenerator(bomb_particle_generator, { "ExplosionForce" });
	addChild(generator);
	generator->trigger();
	return p;
}

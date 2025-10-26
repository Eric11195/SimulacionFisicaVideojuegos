#include "Bomb.hpp"
#include "ForceGenerator.hpp"
#include "ParticleGenerator.hpp"
#include "ParticleGeneratorsDescriptors.hpp"


Bomb::Bomb(Particle::config& c)
	:Particle(c)
{
	addChild(new Variable_ForceGenerator("ExplosionForce", expansion_force,
		[](float force_mag, GameObject const& self, GameObject const& g) {
			const physx::PxVec3 force_applied = (g.get_global_tr().p - self.get_global_tr().p).getNormalized();
			return force_applied * force_mag;
		},
		//Captures force multiplier
		[=](double time, double dt, float& force_mag) {
			force_mag *= force_multiplier * dt;
		}
	));
	auto generator = new TriggeredParticleGenerator(bomb_particle_generator, { "ExplosionForce" });
	addChild(generator);
	generator->trigger();
}

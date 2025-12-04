#pragma once
//#include "ParticleGenerator.hpp"
//#include "ForceGenerator.hpp"
#include "Projectile.hpp"
//#include "ParticleDescriptor.hpp"
#include "ParticleGeneratorsDescriptors.hpp"

constexpr float spring_idle_long = 3.00;
constexpr float elastic_const = 100;
class SpringJoinedProjectileLauncher : public TriggeredParticleGenerator {
public:
	SpringJoinedProjectileLauncher(physx::PxScene* s);
	virtual void generate_particles(double dt, void*) override;
	virtual GameObject* set_up_particle(Particle::config&, void* v) override;
};
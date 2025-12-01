#pragma once
#include "ParticleGenerator.hpp"
#include "GameObject.hpp"

class BombGenerator : public TriggeredParticleGenerator {
public:
	BombGenerator(physx::PxScene* s, float force_mag, float rad, float explosion_force_tao, std::initializer_list<std::string> forces = {});
	virtual GameObject* set_up_particle(Particle::config& p, void* v);
protected:
	const float force_mag;
	//const float force_added_per_second;
	const float rad_squared;
	const float explosion_force_tao;
};
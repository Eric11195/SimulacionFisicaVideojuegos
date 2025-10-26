#pragma once
#include "ParticleGenerator.hpp"
#include "GameObject.hpp"

class BombGenerator : public TriggeredParticleGenerator {
public:
	BombGenerator(ParticleGenerator::config& c, float force_mag, float force_reduction_coef, std::initializer_list<std::string> forces = {});
	virtual Particle* set_up_particle(Particle::config& p);
protected:
	const float force_mag;
	const float force_reduction_coef;
};
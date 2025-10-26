#pragma once
#include "ParticleGenerator.hpp"
#include "ShipCannon.hpp"

/*
class MissileCannon : public ShipCannon {
	MissileCannon(parent_tr);
};
*/

class MissileGenerator : public TriggeredParticleGenerator {
public:
	MissileGenerator();
protected:
	virtual Particle* set_up_particle(Particle::config& p) override;
};
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
	virtual void set_up_particle(Particle* p) override;
};
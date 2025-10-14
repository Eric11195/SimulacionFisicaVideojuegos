#pragma once
#include "ParticleGenerator.hpp"
#include "ShipCannon.hpp"

class MissileCannon : public ShipCannon {
	MissileCannon();
};

class Missile : public TriggeredParticleGenerator {
public:
	Missile();
protected:
	virtual void set_up_particle(Particle* p) override;
};
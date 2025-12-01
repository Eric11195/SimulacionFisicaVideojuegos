#pragma once
#include "ParticleGenerator.hpp"
#include "ShipCannon.hpp"
#include "Projectile.hpp"

/*
class MissileCannon : public ShipCannon {
	MissileCannon(parent_tr);
};
*/

class MissileGenerator : public TriggeredParticleGenerator {
public:
	MissileGenerator(physx::PxScene* s);
protected:
	virtual GameObject* set_up_particle(Particle::config& p, void* v) override;
};

class Missile : public Projectile {
public:
	Missile(physx::PxScene* s, Particle::config&);
	void step(double dt) override;
};
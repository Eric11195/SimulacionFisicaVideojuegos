#pragma once

#include "ParticleGenerator.hpp"
class ForceGenerator;

class ShipRegularProjectileCannon : public TriggeredParticleGenerator {
public:
	ShipRegularProjectileCannon(physx::PxScene* s);
	virtual void* set_up_particle(Particle::config& p) override;
protected:
	ForceGenerator* my_mod_gravity;
};
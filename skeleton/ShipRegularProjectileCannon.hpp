#pragma once

#include "ParticleGenerator.hpp"
class ForceGenerator;

class ShipRegularProjectileCannon : public TriggeredParticleGenerator {
public:
	ShipRegularProjectileCannon(physx::PxScene* s);
	virtual GameObject* set_up_particle(Particle::config& p, void* v) override;
protected:
	ForceGenerator* my_mod_gravity;
};
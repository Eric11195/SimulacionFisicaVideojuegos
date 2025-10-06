#include "ParticleSystem.hpp"

ParticleSystem::ParticleSystem()
	: GlobalCoords_CompositeGameObject(GlobalCoords_CompositeGameObject::config())
{
}

void ParticleSystem::addChild(GameObject* go)
{
	//dynamic cast return nullptr if the cast cannot be done
	ParticleGenerator* pg = dynamic_cast<ParticleGenerator*>(go);
	if (pg==nullptr) {
		throw "Tried to add a non Particle Generator to a ParticleSystem";
	}
}

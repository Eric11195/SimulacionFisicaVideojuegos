#include "ParticleSystem.hpp"

ParticleSystem::ParticleSystem(std::initializer_list<GameObject*> go_s)
	: GlobalCoords_CompositeGameObject(GameObject::config(), go_s)
{
}

void ParticleSystem::addChild(GameObject* go)
{
	//dynamic cast return nullptr if the cast cannot be done
	/*
	ParticleGenerator* pg = dynamic_cast<ParticleGenerator*>(go);
	if (pg==nullptr) {
		throw "Tried to add a non Particle Generator to a ParticleSystem";
	}
	*/
	GlobalCoords_CompositeGameObject::addChild(go);
}

#include "ParticleSystem.hpp"

ParticleSystem::ParticleSystem(Transform const& parent_tr, std::initializer_list<GameObject*> go_s)
	: PassThrough_CompositeGameObject(GameObject::config(), parent_tr, go_s)
{
}
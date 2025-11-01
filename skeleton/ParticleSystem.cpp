#include "ParticleSystem.hpp"

ParticleSystem::ParticleSystem(Transform const& parent_tr, std::initializer_list<GameObject*> go_s)
	: GameObject(GameObject::config(), go_s)
{
}
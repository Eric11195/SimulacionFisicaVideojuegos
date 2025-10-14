#include "ParticleSystem.hpp"

ParticleSystem::ParticleSystem(std::initializer_list<GameObject*> go_s)
	: GameObject(GameObject::config(), go_s)
{
}
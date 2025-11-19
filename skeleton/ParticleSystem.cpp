#include "ParticleSystem.hpp"

ParticleSystem::ParticleSystem(physx::PxScene* s, Transform const& parent_tr, std::initializer_list<GameObject*> go_s)
	: GameObject(s,GameObject::config(), go_s)
{
}
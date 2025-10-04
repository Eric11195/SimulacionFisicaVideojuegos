#include "ParticleSystem.hpp"

ParticleSystem::ParticleSystem(config& c, std::initializer_list<ParticleGenerator::config>& pg)
	: GameObject(Transform(c.origin.turn()))
{
	for (auto i : pg) {

	}
}

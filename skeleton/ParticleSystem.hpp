#pragma once
#include "GameObject.hpp"
#include "My_Vector3.hpp"
#include "ParticleGenerator.hpp"
#include <list>


class ParticleSystem : public GameObject {
public:
	struct config {
		My_Vector3 origin;
	};
	ParticleSystem(config& c, std::initializer_list<ParticleGenerator::config>& pg);
protected:
	std::list<GameObject*> particle_generator_list;
};
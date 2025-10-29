#pragma once
#include "ParticleGenerator.hpp"
#include <list>



class ParticleSystem : public GameObject {
public:
	ParticleSystem(const ParticleSystem&) = delete;
	ParticleSystem& operator =(const ParticleSystem&) = delete;
	//Includes indetermined number of Particle systems inside the constructors, so that it adds them automatically
	ParticleSystem(Transform const& parent_tr, std::initializer_list<GameObject*> = {});
};
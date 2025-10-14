#pragma once
#include "GameObject.hpp"
#include "My_Vector3.hpp"
#include "ParticleGenerator.hpp"
#include <list>
#include "GlobalCoords_CompositeGameObject.hpp"



class ParticleSystem : public GameObject{
public:
	ParticleSystem(const ParticleSystem&) = delete;
	ParticleSystem& operator =(const ParticleSystem&) = delete;
	//Includes indetermined number of Particle systems inside the constructors, so that it adds them automatically
	ParticleSystem(std::initializer_list<GameObject*> = {});
};
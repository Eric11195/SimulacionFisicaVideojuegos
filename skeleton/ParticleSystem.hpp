#pragma once
#include "PassThrough_CompositeGameObject.hpp"
#include "My_Vector3.hpp"
#include "ParticleGenerator.hpp"
#include <list>
#include "GlobalCoords_CompositeGameObject.hpp"



class ParticleSystem : public PassThrough_CompositeGameObject {
public:
	ParticleSystem(const ParticleSystem&) = delete;
	ParticleSystem& operator =(const ParticleSystem&) = delete;
	//Includes indetermined number of Particle systems inside the constructors, so that it adds them automatically
	ParticleSystem(Transform const& parent_tr, std::initializer_list<GameObject*> = {});
};
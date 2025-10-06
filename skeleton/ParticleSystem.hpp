#pragma once
#include "GameObject.hpp"
#include "My_Vector3.hpp"
#include "ParticleGenerator.hpp"
#include <list>
#include "GlobalCoords_CompositeGameObject.hpp"



class ParticleSystem : public GlobalCoords_CompositeGameObject {
public:
	ParticleSystem(const ParticleSystem&) = delete;
	ParticleSystem& operator =(const ParticleSystem&) = delete;
	ParticleSystem();
	virtual void addChild(GameObject* go) override;
};
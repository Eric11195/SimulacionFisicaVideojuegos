#pragma once
#include "ParticleGenerator.hpp"
#include "GameObject.hpp"

class Bomb : public GameObject{
public:
	Bomb(GameObject::config& c, float expansion_force, float force_multiplier);
};
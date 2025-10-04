#pragma once
#include "GameObject.hpp"
#include <list>
#include "My_Vector3.hpp"
#include <cstdint>

class ParticleGenerator : public GameObject {
public:
	struct config {
		My_Vector3 origin;
		std::list<GameObject*>& my_list;
		float average_speed;
		uint8_t particle_generated_in_step;
	};
	ParticleGenerator(config c);
	virtual void step(double dt) override;
	virtual void cleanup() override;
protected:
	std::list<GameObject*> particle_list;
};
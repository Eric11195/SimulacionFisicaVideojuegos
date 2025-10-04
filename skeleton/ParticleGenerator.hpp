#pragma once
#include "GameObject.hpp"
#include <list>
#include "My_Vector3.hpp"
#include <cstdint>

class ParticleGenerator : public GameObject {
public:
	struct config {
		My_Vector3 origin;
		float average_speed;
		uint8_t particle_generated_per_step;
	};
	ParticleGenerator(config c);
	virtual void step(double dt) override;
	virtual void cleanup() override;
protected:
	std::list<GameObject*> particle_list;
	float avrg_speed;
	uint8_t particles_generated_per_step;
	//called when particles would be generated
	virtual void generate_particles();
};

//---------------------------------------------------------------------------------------------------------

class TriggeredParticleGenerator : public ParticleGenerator {
public:
	TriggeredParticleGenerator(ParticleGenerator::config c);
	//Generates particles as specified config
	void Trigger();
};

//---------------------------------------------------------------------------------------------------------

class ToggleParticleGenerator : public ParticleGenerator {
public:
	ToggleParticleGenerator(ParticleGenerator::config c, bool initial_state);
	//Modify step function so that it spawns particles
	inline void set_state(bool new_state) { state = new_state; }
private:
	bool state;
};

//---------------------------------------------------------------------------------------------------------

//Can be adjusted with a multiplier
/*
class AdjustableParticleGenerator : public ToggleParticleGenerator {
private:
float ratio;
};
*/



//IMP
/*
Cada sistema almacena 2 lambdas, una para cuando las particulas se generan, y otra para cuando se destruyen
*/
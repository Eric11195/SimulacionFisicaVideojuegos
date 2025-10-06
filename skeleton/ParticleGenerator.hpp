#pragma once
#include "GameObject.hpp"
#include <list>
#include "My_Vector3.hpp"
#include <cstdint>
#include "GlobalCoords_CompositeGameObject.hpp"
#include "Particle.hpp"
#include <functional>

class ParticleGenerator : public GlobalCoords_CompositeGameObject {
public:
	struct particle_calculator_functions {
		std::function<My_Vector3()> pos = [] { return My_Vector3{ 0,0,0 }; };
		std::function<My_Vector3()> dir = [] { return My_Vector3{0,0,0 }; };
		std::function<float()> vel = [] { return 0; };
	};
	struct config {
		GlobalCoords_CompositeGameObject::config go_config;
		uint8_t particle_generated_per_second;
		Particle::config particle_config;
		particle_calculator_functions particle_lambdas;
	};
	ParticleGenerator(config& c);
	virtual void step(double dt) override;
	virtual void cleanup() override;
protected:
	float avrg_speed;
	uint8_t particle_generated_per_second;
	Particle::config p_config;
	float particles_per_second_accumulator = 0;
	//called when particles would be generated
	virtual void generate_particles(double dt);
	std::function<void(Particle::config&)> next_particle_config_calculator;
	particle_calculator_functions my_particle_lambdas;
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
#pragma once
#include "GameObject.hpp"
#include <list>
#include <cstdint>
#include "GlobalCoords_CompositeGameObject.hpp"
#include "Particle.hpp"
#include <functional>
#include <vector>
#include "inverse_mass.hpp"


/*
Datos random:
	pos inicial
	vel inicial
	lifetime
	color
	tama�o
	(aceleraci�n)
	(damping)
*/

/*
Eliminar particulas cuando:
	lifetime
	posici�n (lluvia llega al suelo)
	salen zona de interes
	n� max
*/


class ParticleGenerator : public GlobalCoords_CompositeGameObject {
public:
	struct particle_calculator_functions {
		std::function<physx::PxVec3()> pos = [] { return physx::PxVec3{ 0,0,0 }; };
		std::function<physx::PxVec3()> dir = [] { return physx::PxVec3{ 0,0,0 }; };
		std::function<float()> vel = [] { return 0; };
		std::function<float()> lifetime = [] { return 0; };
		std::function<Vector4()> color = [] { return Vector4( 0,0,0,0 ); };
		std::function<float()> size = [] { return 0; };
		std::function<bool(PxVec3 pos_particle, PxVec3 pos_generator)> inside_area_of_interest
			= [](PxVec3 pos_particle, PxVec3 pos_generator) {return true; };
		std::function<Mass()> mass = [] {return Mass(0); };
	};
	struct config {
		GlobalCoords_CompositeGameObject::config go_config;
		float particle_generated_per_second;
		Particle::config particle_config;
		particle_calculator_functions particle_lambdas;
	};
	ParticleGenerator(config& c);
	virtual void step(double dt) override;
protected:
	float avrg_speed;
	float avrg_lifetime;
	float avrg_size;
	Mass avrg_mass;
	physx::PxVec3 particle_start_pos;
	Vector4 avrg_color;
	float particle_generated_per_second;
	Particle::config p_config;
	const GameObject::config const_p_config;
	float particles_per_second_accumulator = 0;
	//called when particles would be generated
	virtual void ParticleGenerator::generate_particles(double dt);
	virtual Particle* set_up_particle(Particle::config&);
	std::function<void(Particle::config&)> next_particle_config_calculator;
	particle_calculator_functions my_particle_lambdas;
};

//STARTS WITH FORCE APPLIED => Debug purposes

class ForceAffectedParticleGenerator : public ParticleGenerator {
public:
	ForceAffectedParticleGenerator(ParticleGenerator::config& c, std::initializer_list<std::string> forces, std::initializer_list<ForceGenerator*> forces_ptr = {});
	virtual Particle* set_up_particle(Particle::config& p) override;
protected:
	std::vector<std::string> force_names;
	std::vector<ForceGenerator*> force_ptr;
};


//---------------------------------------------------------------------------------------------------------

class TriggeredParticleGenerator : public ForceAffectedParticleGenerator {
public:
	TriggeredParticleGenerator(ParticleGenerator::config& c, std::initializer_list<std::string> forces = {}, std::initializer_list<ForceGenerator*> forces_ptr = {});
	//Generates particles as specified config
	void trigger();
	void step(double dt) override;
};

//---------------------------------------------------------------------------------------------------------
//NOT IMPLEMENTED

class ToggleParticleGenerator : public ParticleGenerator {
public:
	ToggleParticleGenerator(ParticleGenerator::config& c, bool initial_state);
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
#pragma once
//Elemento base del juego
//Velocidad solo cambia con aceleración
//Métodos:
//	Init -> Inicializador
//	Step -> Update
//	Cleanup -> Borrar
//Integradores:
//	Euler Explicito
//	Semiexplicito
#include "SceneObject.hpp"
#include "PhysicLib.hpp"

#define EULER_SEMI_EXPLICIT_INTEGRATION
//#define EULER_INTEGRATION
#define DAMPING

class Particle : public SphereObject{
public:
	struct config {
		SphereObject::config spho_config;
		//THIS PARAMETER SHOULD BE INSIDE A PARTICLE GENERATOR INSTEAD
		float time_till_death = std::numeric_limits<float>::infinity();
	};
	Particle(config& c);
	/*
	struct config_particle_in_system {
		SceneObject::config scene_obj_config;
		//THIS PARAMETER SHOULD BE INSIDE A PARTICLE GENERATOR INSTEAD
		float time_till_death = std::numeric_limits<float>::infinity();
	};
	Particle(config_particle_in_system& c, PxShape* shape);
	*/
	inline bool alive() {
		return time_till_death > 0;
	};
	
	virtual void step(double dt) override;
private:
	float time_till_death;
};
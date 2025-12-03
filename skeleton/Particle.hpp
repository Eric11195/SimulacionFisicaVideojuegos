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
#include "RigidbodyObject.hpp"

#define EULER_SEMI_IMPLICIT_INTEGRATION
//#define EULER_INTEGRATION
#define DAMPING

class Particle : public SphereObject{
public:
	struct config {
		SphereObject::config spho_config;
		//THIS PARAMETER SHOULD BE INSIDE A PARTICLE GENERATOR INSTEAD
		float time_till_death = std::numeric_limits<float>::infinity();
	};
	Particle(physx::PxScene* s, config& c);
	virtual bool alive() override {
		return time_till_death > 0;
	};
	
	virtual void step(double dt) override;

protected:
	float time_till_death;
};

class RigidParticle : public Rigid_SphereObject {
public:
	RigidParticle(physx::PxScene*, Particle::config& c);
	virtual void step(double dt) override;
	virtual bool alive() override {
		return time_till_death > 0;
	};
protected:
	float time_till_death;
};
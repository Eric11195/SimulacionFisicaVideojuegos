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
#include "My_Vector3.hpp"
#include "PhysicLib.hpp"

#define EULER_SEMI_EXPLICIT_INTEGRATION
//#define EULER_INTEGRATION
#define DAMPING

class Particle : public SphereObject{
public:
	Particle(My_Vector3 _pos, My_Vector3 _vel, float radius);
	//~Particle();
	void init() override;
	virtual void step(double dt) override;
	void cleanup() override;
	void set_accel(My_Vector3 new_accel);
	void add_accel(My_Vector3 add_accel);
#ifdef DAMPING
	void set_dumping(float f);
#endif
protected:
	//updates speed and position
	void integrate(double t);
private:
#ifdef DAMPING
	float damping_mult = PhysicLib::NORMAL_DAMPING;
#endif
	My_Vector3 vel;
	My_Vector3 accel;
};
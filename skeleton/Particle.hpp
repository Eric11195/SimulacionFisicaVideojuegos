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
#include "GameObject.hpp"
#include "My_Vector3.hpp"

#define EULER_SEMI_EXPLICIT_INTEGRATION
//#define EULER_INTEGRATION
#define DAMPING

#ifdef DAMPING
constexpr float DAMPING_MULT = 0.9999;
#endif

constexpr float PARTICLE_RADIUS = 5;


class Particle : public SphereObject{
public:
	Particle(My_Vector3 _pos, My_Vector3 _vel);
	//~Particle();
	void init() override;
	void step(double dt) override;
	void cleanup() override;
	void change_accel(My_Vector3 new_accel);
protected:
	//updates speed and position
	void integrate(double t);
private:
	My_Vector3 vel;
	My_Vector3 accel;
};
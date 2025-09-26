#pragma once
//Elemento base del juego
//Velocidad solo cambia con aceleraci�n
//M�todos:
//	Init -> Inicializador
//	Step -> Update
//	Cleanup -> Borrar
//Integradores:
//	Euler Explicito
//	Semiexplicito
#include "GameObject.hpp"
#include "My_Vector3.hpp"

constexpr float PARTICLE_RADIUS = 2;


class Particle : SphereObject{
public:
	Particle(My_Vector3 _pos, My_Vector3 _vel);
	~Particle();
	void init() {

	}
	void step() {

	}
	void cleanup() {

	}
private:
	My_Vector3 vel;
};
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
	struct config {
		My_Vector3 pos, initial_dir;
		float initial_speed;
		float radius;
		float time_till_death = std::numeric_limits<float>::infinity();
		My_Vector3 accel = { 0,0,0 };
	};
	Particle(config& c);
	inline bool alive() {
		return time_till_death > 0;
	};
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
	float time_till_death;
	My_Vector3 vel;
	My_Vector3 accel;
};
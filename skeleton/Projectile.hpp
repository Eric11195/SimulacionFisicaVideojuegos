#pragma once
#include "Particle.hpp"
#include "PhysicLib.hpp"

using namespace PhysicLib;

/* PARA CHOQUES CON MISMO EFECTO
Energía real = Energía simulada
mr * vr^2 = ms * sv^2
ms = mr * (vr^2 / vs^2)
*/

/* PARA MISMO ALCANCE
Distance = (2*h/gravedad)^(1/2)
Hay q cambiar la gravedad g
gs = gr*(vs^2/vr^2)
*/

/* DISPARAR PROYETILES DESDE LA CÁMARA
* GetCamera->getDir();	 //Dirección cámara
* GetCamera()->getEye(); //centro cámara
*/


class Projectile : public RigidParticle {
public:
	//modifies its own mass, and mutiplies force_applied to gravity
	Projectile(physx::PxScene* s, Particle::config& c, float real_speed, float simulated_speed);
	~Projectile();
	float get_gravity_proportion(float real_speed, float sim_speed);
	float get_s_mass(float mass, float real_speed, float sim_speed);
	virtual void step(double dt) override;
	//virtual void integrate(double dt) override;
protected:
	float gravity_multiplier;
};
#pragma once
#include "Particle.hpp"
#include "PhysicLib.hpp"
#include "My_Vector3.hpp"

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


class Projectile : public Particle {
public:
	struct projectile_config {
		My_Vector3 position, initial_dir;
		speed s_real, s_simulated;
		mass m_real;
		float radius;
		gravity m_gravity = PhysicLib::GRAVITY;
		float damping = PhysicLib::NORMAL_DAMPING;
	};
	virtual void step(double dt) override;
	Projectile(projectile_config c);
protected:
	mass m_real;
	mass m_simulated;
	speed s_real;
	speed s_simulated;
	gravity g_simulated;
	gravity g_real;
	void update_s_gravity();
	void update_s_mass();
};
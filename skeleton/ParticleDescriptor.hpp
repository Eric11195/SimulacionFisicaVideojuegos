#pragma once
#include "Particle.hpp"
#include "PhysicLib.hpp"

namespace ParticleDescriptor {
	Particle::config regular_ball{
		SphereObject::config{
			SceneObject::config{
				GameObject::config{
					{0,0,0},//POS
					{0,1,0},//VEL DIR
					//{0,-1,0},//ACCEL_DIR
					5, //VEL MAG
					//PhysicLib::GRAVITY //ACELL MOD
					//DAMPING MULT
				},
				{1,1,1,1}//COLOR
			},
			2//SIZE
		},
		4 //TIME TILL DESTROY
	};
}
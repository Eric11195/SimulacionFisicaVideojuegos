#pragma once
#include "ParticleGenerator.hpp"
#include "Distributions.hpp"

namespace ParticleGeneratorsDescriptors {
	const ParticleGenerator::config ball_thrower {
		4, //Particles per second
		Particle::config{
			{//SphO_config
				{//SceneObject config
					{//GameObject config
						{0,0,0}, //Pos
						{0,1,0}, //speed_dir
						{0,-1,0},//accel_dir
						30, //Speed module
						PhysicLib::GRAVITY,//Accel module
					}	
					//Color
				},
				1 //rad
			},
			4 //lifetime
		},
		ParticleGenerator::particle_calculator_functions{
			[] {return My_Vector3{0,0,0}; },//POS
			[] {//VEL_DIR
				return My_Vector3{
					0.5f * Distributions::NormalDistribution::get(),
					1,
					0.5f * Distributions::NormalDistribution::get()
				};
			},
			[] {//SPEED MOD
				return Distributions::RandomSignDistribution::get() * Distributions::LinearDistribution::get() * 5;
			}
		}
	};
}
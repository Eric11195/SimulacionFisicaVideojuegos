#pragma once
#include "ParticleGenerator.hpp"
#include "Distributions.hpp"

namespace ParticleGeneratorsDescriptors {
	ParticleGenerator::config ball_thrower {
		GameObject::config{
			{0,0,0}, //Pos
			{0,0,1}, //speed_dir
			{0,-1,0},//accel_dir
			0, //Speed module
			0//PhysicLib::GRAVITY,//Accel module
		},
		20, //Particles per second
		Particle::config{
			{//SphO_config
				{//SceneObject config
					{//GameObject config
						{0,0,0}, //Pos
						{0,1,0}, //speed_dir
						{0,-1,0},//accel_dir
						20, //Speed module
						PhysicLib::GRAVITY,//Accel module
					},	
					physx::PxVec4(0,0,0,0)
					//Color
				},
				1 //rad
			},
			2 //lifetime
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
			},
			[] {//Lifetime MOD
				return Distributions::NormalDistribution::get() * 1 * Distributions::RandomSignDistribution::get();
			},
			[] {//Vector4
				return Vector4(
					Distributions::LinearDistribution::get(),
					Distributions::LinearDistribution::get(),
					Distributions::LinearDistribution::get(),
					1//Distributions::LinearDistribution::get()
				);
			},
			[] {//Size
				return 0.5*Distributions::NormalDistribution::get();
			}
		}
	};
}
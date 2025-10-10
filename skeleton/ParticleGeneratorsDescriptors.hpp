#pragma once
#include "ParticleGenerator.hpp"
#include "Distributions.hpp"

using namespace Distributions;

namespace ParticleGeneratorsDescriptors {
	ParticleGenerator::config ball_thrower{
		GameObject::config{
			{0,0,0}, //Pos
			{0,0,1}, //speed_dir
			{0,-1,0},//accel_dir
			5, //Speed module
			0,//PhysicLib::GRAVITY,//Accel module
			PhysicLib::NORMAL_DAMPING,//DAMPING
			//physx::PxQuat(1.0f, 0.0f,0.0f,0.0f)
		},
		20, 
		//Particles per second
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
			[] {return My_Vector3{
				Distributions::RandomSignDistribution::get() * Distributions::LinearDistribution::get() * 5,
				Distributions::RandomSignDistribution::get() * Distributions::LinearDistribution::get() * 5,
				Distributions::RandomSignDistribution::get() * Distributions::LinearDistribution::get() * 5
				};
			},//POS
			[] {//VEL_DIR
				return My_Vector3{
					0.1f * Distributions::NormalDistribution::get(NormalDistribution::d_025),
					1,
					0.1f * Distributions::NormalDistribution::get(NormalDistribution::d_025)
				};
			},
			[] {//SPEED MOD
				return Distributions::RandomSignDistribution::get() * Distributions::LinearDistribution::get() * 5;
			},
			[] {//Lifetime MOD
				return Distributions::NormalDistribution::get(NormalDistribution::d_05) * 1 * Distributions::RandomSignDistribution::get();
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
				return Distributions::NormalDistribution::get(NormalDistribution::d_025);
			},
			[](Vector3 pos_particle, Vector3 pos_generator) {//Area of interest
				//Inside radius of parent
				auto vector_from_particle_to_generator = pos_generator - pos_particle;
				auto module = vector_from_particle_to_generator.magnitude();
				return module < 10;
			}
		}
	};
}
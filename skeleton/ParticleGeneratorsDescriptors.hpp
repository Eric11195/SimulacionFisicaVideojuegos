#pragma once
#include "ParticleGenerator.hpp"
#include "Distributions.hpp"

/*------GENERATOR
		uint8_t particle_generated_per_second;
		Particle::config particle_config;
		particle_calculator_functions particle_lambdas;

--------PARTICLE
		My_Vector3 pos, initial_dir;
		float initial_speed;
		float radius;
		float time_till_death = std::numeric_limits<float>::infinity();
		My_Vector3 accel = { 0,0,0 };
--------FUNCTIONS
		std::function<My_Vector3()> pos = [] { return My_Vector3{ 0,0,0 }; };
		std::function<My_Vector3()> dir = [] { return My_Vector3{0,0,0 }; };
		std::function<float()> vel = [] { return 0; };
*/

namespace ParticleGeneratorsDescriptors {
	const ParticleGenerator::config ball_thrower {
		4,
		Particle::config{
			My_Vector3{0,0,0}, My_Vector3{0,1,0},
			30,
			1,
			4,
			My_Vector3{0,-10,0}
		},
		ParticleGenerator::particle_calculator_functions{
			[] {return My_Vector3{0,0,0}; },
			[] {
				return My_Vector3{
					0.5f * Distributions::NormalDistribution::get(),
					1,
					0.5f * Distributions::NormalDistribution::get()
				};
			},
			[] {
				return Distributions::RandomSignDistribution::get() * Distributions::LinearDistribution::get() * 5;
			}
		}
	};
}
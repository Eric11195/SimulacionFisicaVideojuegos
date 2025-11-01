#include "ParticleGeneratorsDescriptors.hpp"


ParticleGenerator::config bomb{
	1,
	//Particles per second
	bomb_init_particle(),
	ParticleGenerator::particle_calculator_functions{
		[] {return physx::PxVec3{
				0,0,0
			};
		},//POS
		[] {//VEL_DIR
			return physx::PxVec3{
				0,
				0,
				0
			};
		},
		[] {//SPEED MOD
			return 0;
		},
		[] {//Lifetime MOD
			return 0;
		},
		[] {//Vector4
			return Vector4(
				0,
				0,
				0,
				1//Distributions::LinearDistribution::get()
			);
		},
		[] {//Size
			return 0;
		},
		[](PxVec3 pos_particle, PxVec3 pos_generator) {//Area of interest
		//Inside radius of parent;
		return true;
	}
}

};


ParticleGenerator::config bomb_particle_generator{
	100,
	//Particles per second
	bomb_created_particles(),
	ParticleGenerator::particle_calculator_functions{
		[] {return physx::PxVec3{
				Distributions::LinearDistribution::get() * 1 -0.5f,
				Distributions::LinearDistribution::get() * 1 -0.5f,
				Distributions::LinearDistribution::get() * 1 -0.5f
			};
		},//POS
		[] {//VEL_DIR
			return physx::PxVec3{
				0,
				0,
				0
			};
		},
		[] {//SPEED MOD
			return 0;
		},
		[] {//Lifetime MOD
			return 0;
		},
		[] {//Vector4
			return Vector4(
				0,
				0,
				0,
				0//Distributions::LinearDistribution::get()
			);
		},
		[] {//Size
			return 0;
		},
		[](PxVec3 pos_particle, PxVec3 pos_generator) {//Area of interest
		//Inside radius of parent;
		return true;
		},
		[] {
			float dist_ret = LinearDistribution::get();
			float new_mass = max(0,1 * dist_ret);
			return Mass(new_mass);
		}
}

};


ParticleGenerator::config testing_blackhole_particles{
	100,
	//Particles per second
	black_hole_particles(),
	ParticleGenerator::particle_calculator_functions{
		[] {return physx::PxVec3{
				Distributions::LinearDistribution::get()*10,
				Distributions::LinearDistribution::get()*10,
				Distributions::LinearDistribution::get()*10
			};
		},//POS
		[] {//VEL_DIR
			return physx::PxVec3{
				0,
				0,
				0
			};
		},
		[] {//SPEED MOD
			return 0;
		},
		[] {//Lifetime MOD
			return 0;
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
			return 0;
		},
		[](PxVec3 pos_particle, PxVec3 pos_generator) {//Area of interest
		//Inside radius of parent;
		return true;
		},
		[] {
			if (RandomSignDistribution::get() > 0) {
				return Mass(8);
			}
			else return Mass(0.1);
			//return Mass(LinearDistribution::get() * 2);
		}
}

};

ParticleGenerator::config ball_thrower{
	20,
	//Particles per second
	regular_ball(),
	ParticleGenerator::particle_calculator_functions{
		[] {return physx::PxVec3{
			Distributions::RandomSignDistribution::get() * Distributions::LinearDistribution::get() * 5,
			Distributions::RandomSignDistribution::get() * Distributions::LinearDistribution::get() * 5,
			Distributions::RandomSignDistribution::get() * Distributions::LinearDistribution::get() * 5
			};
		},//POS
		[] {//VEL_DIR
			return physx::PxVec3{
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
		[](PxVec3 pos_particle, PxVec3 pos_generator) {//Area of interest
		//Inside radius of parent
		auto vector_from_particle_to_generator = pos_generator - pos_particle;
		auto module = vector_from_particle_to_generator.magnitude();
		return module < 10;
	}
}
};

ParticleGenerator::config x_wing_shoot_type{
	1,//Particles per second
	laser_shot(),
	ParticleGenerator::particle_calculator_functions{
	[] {return physx::PxVec3{0,0,0
		/*
			5*Distributions::NormalDistribution::get(NormalDistribution::d_10) * Distributions::RandomSignDistribution::get(), //0,0,0
			5*Distributions::NormalDistribution::get(NormalDistribution::d_10)* Distributions::RandomSignDistribution::get(),
			5*Distributions::NormalDistribution::get(NormalDistribution::d_10)* Distributions::RandomSignDistribution::get()
			*/
		};
	},//POS
	[] {//VEL_DIR
		return physx::PxVec3{
			0,0,1
		};
	},
	[] {//SPEED MOD
		return 5;// Distributions::RandomSignDistribution::get()* Distributions::LinearDistribution::get() * 5;
	},
	[] {//Lifetime MOD
		return 0;//Distributions::NormalDistribution::get(NormalDistribution::d_05) * 1 * Distributions::RandomSignDistribution::get();
	},
	[] {//Vector4
		return Vector4(
			0,0,0,
			1//Distributions::LinearDistribution::get()
		);
	},
	[] {//Size
		return 0;//Distributions::NormalDistribution::get(NormalDistribution::d_025);
	},
	[](PxVec3 pos_particle, PxVec3 pos_generator) {//Area of interest
		//Inside radius of parent
		auto vector_from_particle_to_generator = pos_generator - pos_particle;
		auto module = vector_from_particle_to_generator.magnitude();
		return module < 100;
	}
}
};
ParticleGenerator::config missile{
		1,//Particles per second
		missile_particle(),
		ParticleGenerator::particle_calculator_functions{
			[] {return physx::PxVec3{ 0,0,0};
			},//POS
			[] {//VEL_DIR
				return physx::PxVec3{
					0,0,1
				};
			},
			[] {//SPEED MOD
				return 0;// Distributions::RandomSignDistribution::get()* Distributions::LinearDistribution::get() * 5;
			},
			[] {//Lifetime MOD
				return 0;//Distributions::NormalDistribution::get(NormalDistribution::d_05) * 1 * Distributions::RandomSignDistribution::get();
			},
			[] {//Vector4
				return Vector4(
					0,0,0,
					1//Distributions::LinearDistribution::get()
				);
			},
			[] {//Size
				return 0;//Distributions::NormalDistribution::get(NormalDistribution::d_025);
			},
			[](PxVec3 pos_particle, PxVec3 pos_generator) {//Area of interest
		//Inside radius of parent
		auto vector_from_particle_to_generator = pos_generator - pos_particle;
		auto module = vector_from_particle_to_generator.magnitude();
		return module < 1000;
	}
}
};
ParticleGenerator::config missile_particle_system{
	50,
	//Particles per second
	missile_generated_particles(),
ParticleGenerator::particle_calculator_functions{
	[] {return physx::PxVec3{
	0.25f*Distributions::RandomSignDistribution::get() * Distributions::NormalDistribution::get(NormalDistribution::d_025),
	0.25f*Distributions::RandomSignDistribution::get() * Distributions::NormalDistribution::get(NormalDistribution::d_025),
	0.0f//Distributions::RandomSignDistribution::get() * Distributions::NormalDistribution::get(NormalDistribution::d_025)
	};
},//POS
[] {//VEL_DIR
	return physx::PxVec3{
		0.25f * Distributions::NormalDistribution::get(NormalDistribution::d_025),
		0.25f * Distributions::NormalDistribution::get(NormalDistribution::d_025),
		-1,
	};
},
[] {//SPEED MOD
	return Distributions::RandomSignDistribution::get() * Distributions::NormalDistribution::get(NormalDistribution::d_05);
},
[] {//Lifetime MOD
	return 0.25*Distributions::NormalDistribution::get(NormalDistribution::d_05) * Distributions::RandomSignDistribution::get();
},
[] {//Vector4
	return Vector4( 0,Distributions::LinearDistribution::get(),0,0
		//Distributions::LinearDistribution::get(),
		//Distributions::LinearDistribution::get(),
		//Distributions::LinearDistribution::get(),
		//1//Distributions::LinearDistribution::get()
	);
},
[] {//Size
	return 0.03*Distributions::NormalDistribution::get(NormalDistribution::d_025);
},
[](PxVec3 pos_particle, PxVec3 pos_generator) {//Area of interest
		/*
		//Inside radius of parent
		auto vector_from_particle_to_generator = pos_generator - pos_particle;
		auto module = vector_from_particle_to_generator.magnitude();
		return module < 1000;
		*/
		return true;
	}
}
};

ParticleGenerator::config propulsores_enemy_ship{
	50,
	//Particles per second
	propulsores_enemy_ship_particles(),
ParticleGenerator::particle_calculator_functions{
	[] {return physx::PxVec3{
	1 * Distributions::RandomSignDistribution::get() * Distributions::LinearDistribution::get(),
	1 * Distributions::RandomSignDistribution::get() * Distributions::LinearDistribution::get(),
	0.0f//Distributions::RandomSignDistribution::get() * Distributions::NormalDistribution::get(NormalDistribution::d_025)
	};
},//POS
[] {//VEL_DIR
	return physx::PxVec3{
		0.5f * Distributions::NormalDistribution::get(NormalDistribution::d_025),
		0.5f * Distributions::NormalDistribution::get(NormalDistribution::d_025),
		-1,
	};
},
[] {//SPEED MOD
	return Distributions::RandomSignDistribution::get() * Distributions::NormalDistribution::get(NormalDistribution::d_05);
},
[] {//Lifetime MOD
	return 0.25 * Distributions::NormalDistribution::get(NormalDistribution::d_05) * Distributions::RandomSignDistribution::get();
},
[] {//Vector4
	return Vector4(0,Distributions::LinearDistribution::get(),0,0
		//Distributions::LinearDistribution::get(),
		//Distributions::LinearDistribution::get(),
		//Distributions::LinearDistribution::get(),
		//1//Distributions::LinearDistribution::get()
	);
},
[] {//Size
	return 0.03 * Distributions::NormalDistribution::get(NormalDistribution::d_025);
},
[](PxVec3 pos_particle, PxVec3 pos_generator) {//Area of interest
		/*
		//Inside radius of parent
		auto vector_from_particle_to_generator = pos_generator - pos_particle;
		auto module = vector_from_particle_to_generator.magnitude();
		return module < 1000;
		*/
		return true;
	}
}
};
#include "ParticleGeneratorsDescriptors.hpp"


ParticleGenerator::config bomb{
	GameObject::config{
		{0,0,0}, //Pos
		{0,0,1}, //speed_dir
		//{0,-1,0},//accel_dir
		0, //Speed module
		//0,//PhysicLib::GRAVITY,//Accel module
		PhysicLib::NORMAL_DAMPING,//DAMPING
		//physx::PxQuat(1.0f, 0.0f,0.0f,0.0f)
	},
	1,
	//Particles per second
	Particle::config{
		{//SphO_config
			{//SceneObject config
				{//GameObject config
					{0,0,10}, //Pos
					{0,1,0}, //speed_dir
					//{0,-1,0},//accel_dir
					0, //Speed module
					//PhysicLib::GRAVITY,//Accel module
				},
				physx::PxVec4(0,0,0,0)
				//Color
			},
			1 //rad
		},
		2 //lifetime
	},
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
	}
}

};


ParticleGenerator::config bomb_particle_generator{
	GameObject::config{
		{0,0,1}, //Pos
		{0,0,1}, //speed_dir
		//{0,-1,0},//accel_dir
		0, //Speed module
		//0,//PhysicLib::GRAVITY,//Accel module
		PhysicLib::NORMAL_DAMPING,//DAMPING
		//physx::PxQuat(1.0f, 0.0f,0.0f,0.0f)
	},
	100,
	//Particles per second
	Particle::config{
		{//SphO_config
			{//SceneObject config
				{//GameObject config
					{0,0,0}, //Pos
					{0,1,0}, //speed_dir
					//{0,-1,0},//accel_dir
					0, //Speed module
					//PhysicLib::GRAVITY,//Accel module
				},
				physx::PxVec4(0,0,0,0)
				//Color
			},
			1.0f //rad
		},
		2 //lifetime
	},
	ParticleGenerator::particle_calculator_functions{
		[] {return physx::PxVec3{
				Distributions::LinearDistribution::get() * 5 -2.5f,
				Distributions::LinearDistribution::get() * 5 -2.5f,
				Distributions::LinearDistribution::get() * 5 -2.5f
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
	}
}

};


ParticleGenerator::config testing_blackhole_particles{
	GameObject::config{
		{0,0,0}, //Pos
		{0,0,1}, //speed_dir
		//{0,-1,0},//accel_dir
		0, //Speed module
		//0,//PhysicLib::GRAVITY,//Accel module
		PhysicLib::NORMAL_DAMPING,//DAMPING
		//physx::PxQuat(1.0f, 0.0f,0.0f,0.0f)
	},
	1,
	//Particles per second
	Particle::config{
		{//SphO_config
			{//SceneObject config
				{//GameObject config
					{0,0,0}, //Pos
					{0,1,0}, //speed_dir
					//{0,-1,0},//accel_dir
					0, //Speed module
					//PhysicLib::GRAVITY,//Accel module
				},
				physx::PxVec4(0,0,0,0)
				//Color
			},
			0.1f //rad
		},
		10 //lifetime
	},
	ParticleGenerator::particle_calculator_functions{
		[] {return physx::PxVec3{
				Distributions::LinearDistribution::get() * 5,
				Distributions::LinearDistribution::get() * 5,
				Distributions::LinearDistribution::get() * 5
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
	}
}

};

ParticleGenerator::config ball_thrower{
	GameObject::config{
		{0,0,0}, //Pos
		{0,0,1}, //speed_dir
		//{0,-1,0},//accel_dir
		5, //Speed module
		//0,//PhysicLib::GRAVITY,//Accel module
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
					//{0,-1,0},//accel_dir
					20, //Speed module
					//PhysicLib::GRAVITY,//Accel module
				},
				physx::PxVec4(0,0,0,0)
				//Color
			},
			1 //rad
		},
		2 //lifetime
	},
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
	GameObject::config{
		{0,0,0}, //Pos
		{0,0,1}, //speed_dir
		//{0,-1,0},//accel_dir
		0, //Speed module
		//0,//PhysicLib::GRAVITY,//Accel module
		PhysicLib::NORMAL_DAMPING,//DAMPING
		//physx::PxQuat(1.0f, 0.0f,0.0f,0.0f)
	},
	1,//Particles per second
	Particle::config{
		{//SphO_config
			{//SceneObject config
				{//GameObject config
					{0,0,0}, //Pos
					{0,0,1}, //speed_dir
					//{0,-1,0},//accel_dir
					40, //Speed module
					//0//PhysicLib::GRAVITY,//Accel module
				},
				physx::PxVec4(1,0,0,1)
	//Color
},
0.25 //rad
},
5 //lifetime
},
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
	GameObject::config{
		{0,0,0}, //Pos
		{0,0,1}, //speed_dir
		//{0,-1,0},//accel_dir
		0, //Speed module
		0,//PhysicLib::GRAVITY,//Accel module
		//PhysicLib::NORMAL_DAMPING,//DAMPING
		//physx::PxQuat(1.0f, 0.0f,0.0f,0.0f)
},
1,//Particles per second
Particle::config{
	{//SphO_config
		{//SceneObject config
			{//GameObject config
				{0,0,0}, //Pos
				{0,0,1}, //speed_dir
				//{0,-1,0},//accel_dir
				6,//10, //Speed module
				//0//PhysicLib::GRAVITY,//Accel module
			},
			physx::PxVec4(1,1,1,1)//Color
},
0.2 //rad
},
7 //lifetime
	},
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
	GameObject::config{
		{0,0,0}, //Pos
		{0,0,-1}, //speed_dir
		//{0,0,1},//accel_dir
		0,//5, //Speed module
		//0,//2.5,//PhysicLib::GRAVITY,//Accel module
		PhysicLib::NORMAL_DAMPING,//DAMPING
		//physx::PxQuat(1.0f, 0.0f,0.0f,0.0f)
	},
	50,
	//Particles per second
	Particle::config{
		{//SphO_config
			{//SceneObject config
				{//GameObject config
					{0,0,0}, //Pos
					{0,0,0}, //speed_dir
					//{0,0,-1},//accel_dir
					5,//5, //Speed module
					//0,//ACCEL
					PhysicLib::NORMAL_DAMPING//2.5//DUMPING
				},
				physx::PxVec4(1,0,0,1)
	//Color
},
0.05 //rad
},
0.5 //lifetime
},
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
	return 0.01*Distributions::NormalDistribution::get(NormalDistribution::d_025);
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
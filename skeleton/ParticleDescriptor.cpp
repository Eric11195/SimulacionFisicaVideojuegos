#include "ParticleDescriptor.hpp"

Particle::config& regular_ball()
{
	static Particle::config p{
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
	return p;
}

Particle::config& bomb_init_particle()
{
	static Particle::config p{
	{//SphO_config
		SceneObject::config{//SceneObject config
			GameObject::config{//GameObject config
				{0,0,20}, //Pos
				{0,1,0}, //speed_dir
				//{0,-1,0},//accel_dir
				0, //Speed module

				Mass(100) //mass
				//PhysicLib::GRAVITY,//Accel module
		},
		physx::PxVec4(0,0,0,1)
		//Color
			},
			0.0001 //rad
		},
		1.5f //lifetime
	};
	return p;
};

Particle::config& bomb_created_particles()
{
	static Particle::config p{
	{//SphO_config
		{//SceneObject config
			{//GameObject config
				{0,0,0}, //Pos
				{0,1,0}, //speed_dir
				//{0,-1,0},//accel_dir
				0, //Speed module
				Mass(0.1)//mass
				//PhysicLib::GRAVITY,//Accel module
			},
			physx::PxVec4(0,0,0,0) //Color
		},
	0.1f //rad
	},
	10 //lifetime
	};
	return p;
}

Particle::config& black_hole_particles()
{
	static Particle::config p{
	{//SphO_config
		{//SceneObject config
			{//GameObject config
				{0,0,0}, //Pos
				{0,1,0}, //speed_dir
				//{0,-1,0},//accel_dir
				0, //Speed module
				Mass(3)//PhysicLib::GRAVITY,//Accel module
			},
			physx::PxVec4(0,0,0,0)
			//Color
		},
		0.6 //rad
	},
	3 //lifetime
	};
	return p;
}
Particle::config& laser_shot()
{
	static
		Particle::config p{
				{//SphO_config
					{//SceneObject config
						{//GameObject config
							{0,0,0}, //Pos
							{0,0,1}, //speed_dir
							//{0,-1,0},//accel_dir
							40, //Speed module
							//0//PhysicLib::GRAVITY,//Accel module
						},
						physx::PxVec4(0.937,1,0,1)
						//Color
					},
					0.25 //rad
				},
				5 //lifetime
	};
	return p;
}
Particle::config& missile_particle()
{
	static
		Particle::config p{
{//SphO_config
	{//SceneObject config
		{//GameObject config
			{0,0,0}, //Pos
			{0,0,1}, //speed_dir
			//{0,-1,0},//accel_dir
			20,//10, //Speed module
			//0//PhysicLib::GRAVITY,//Accel module
		},
		physx::PxVec4(1,1,1,1)//Color
	},
	0.2 //rad
},
4 //lifetime
	};
	return p;
}
Particle::config& missile_generated_particles()
{
	static
		Particle::config p{
			{//SphO_config
				{//SceneObject config
					{//GameObject config
						{0,0,0}, //Pos
						{0,0,0}, //speed_dir
						//{0,0,-1},//accel_dir
						5,//5, //Speed module
						//0,//ACCEL
						10//MASS
						//PhysicLib::NORMAL_DAMPING//2.5//DUMPING
					},
					physx::PxVec4(1,0,0,1)
		//Color
	},
	0.05 //rad
},
1 //lifetime
	};
	return p;
}
Particle::config& propulsores_enemy_ship_particles()
{
	static
		Particle::config p{
			{//SphO_config
				{//SceneObject config
					{//GameObject config
						{0,0,-1}, //Pos
						{0,0,-1}, //speed_dir
						10,//5, //Speed module
						//0,//ACCEL
						Mass(0.1)//MASS
						//PhysicLib::NORMAL_DAMPING//2.5//DUMPING
					},
					physx::PxVec4(0.11f,0.333,1,1)
		//Color
	},
	0.4 //rad
},
0.5 //lifetime
	};
	return p;
}
;
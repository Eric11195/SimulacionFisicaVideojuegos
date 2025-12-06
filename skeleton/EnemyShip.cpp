#include "EnemyShip.hpp"
#include "SceneObject.hpp"
#include "Distributions.hpp"
#include <iostream>
#include <cmath>
#include <ctgmath>
#include "ParticleGeneratorsDescriptors.hpp"
#include "ParticleGenerator.hpp"
#include "ForceGenerator.hpp"
#include <cassert>
#include "BombGenerator.hpp"

constexpr float near_threshold_to_flee = 12;

EnemyShip::EnemyShip(physx::PxScene* s, GameObject* player)
	: Rigid_CubeObject(s, CubeObject::config{ {SceneObject::config{GameObject::config{},{0,0,0,0}}}, {1,1,1} }, NO_REPRESENTATION::no_representation), 
	player_go(player)//, my_scene(s)

{
	set_actor_flags(PxActorFlag::eDISABLE_GRAVITY, true);
	rb->setLinearVelocity({ 0,0,0 });
	rb->setAngularVelocity({ 0,0,0 });
	rb->setAngularDamping(0.9);
	rb->setLinearDamping(0.2);

	Mass my_mass = 500;
	mass = InvMass(my_mass);

	SphereObject::config sph_c = { SceneObject::config(), 1 };
	auto n = new SphereObject(s, sph_c);
	n->set_color({ 1, 0.682, 0, 1});
	addChild(n);

	CubeObject::config c_c = { SceneObject::config(), {0.1, 1.5, 1.5} };
	for (auto i = -1; i < 2; i = i + 2) {
		auto cube = new CubeObject(s, c_c);
		cube->translate({ i * 1.2f,0,0 });
		cube->set_color({ 1, 0.682, 0, 1 });
		addChild(cube);
	}
	SceneObject::config c{};
	c.color = {1,0,0,1};
	c_c.half_extents = {0.1, 0.1, 1};
	auto cube = new CubeObject(s,c_c);
	cube->set_color({0.11f, 0.333f, 1, 1});
	cube->translate({ 0,0,0.5 });
	addChild(cube);
	
	//Set random pos
	translate_to({
		(Distributions::LinearDistribution::get() * 100) - 50,
		(Distributions::LinearDistribution::get() * 100) - 50,
		(Distributions::LinearDistribution::get() * 100) - 50
	});
	//set_velocity({ 0,0,15 });
	auto estela_motor = new ToggleParticleGenerator(s,propulsores_enemy_ship);
	estela_motor->set_toggle(true);
	addChild(estela_motor);

	propulsors = new Directional_ForceGenerator(s,{0,0,1}, 12*my_mass.mass);
	addChild(propulsors);
	add_force_to_myself(propulsors);
	//add_force_to_myself("black_hole");
}

void EnemyShip::step(double dt)
{
	if (dead) {
		time_till_definitive_dead -= dt;
		if(time_till_definitive_dead < 0) {
			//Die for good
			die();
			to_be_destroyed = true;
			return;
		}
		else if (time_till_definitive_dead < 0.5 ) {
			//Explode
			if (!exploded) {
				exploded = true;
				explosion->setTransform(global_transform);
				explosion->trigger(nullptr);
			}
		}
	}
	else {
		think_step(dt);
	}
	setTransform(rb->getGlobalPose());
	integrate(dt);

	int i = 0;
	float x_wing_offset = 1.2f;
	physx::PxVec3 x_axis = global_transform.q.rotate({1,0,0});
	physx::PxVec3 z_axis = global_transform.q.rotate({ 0,0,1 });
	for (auto& child : child_objects) {
		switch (i) {
		case 0: //SPHERE
		case 4: //PARTICLE GEN
		case 5: //PROPULSORS
			child->setTransform(global_transform);
			break;
		case 1: //WING LEFT
		case 2: //WING RIGHT
		{
			x_wing_offset *= -1;
			Transform aux_tr = global_transform;
			aux_tr.p += x_axis*x_wing_offset;
			child->setTransform(aux_tr);
			break;
		}
		case 3: {//PITORRO
			Transform aux_tr = global_transform;
			aux_tr.p += z_axis * x_wing_offset;
			child->setTransform(aux_tr);
			break;
		}
		case 6:
			break;
		default: {
			//Its one of the particle system that show us fire where it has been shot
			assert((i - 7) < points_impacted.size());
			Transform aux_tr = global_transform;
			aux_tr.p += aux_tr.q.rotate(points_impacted[i - 7].pos);
			aux_tr.q = global_transform.q * points_impacted[i - 7].looking_to;
			child->setTransform(aux_tr);
			break;
		}
		}
		child->step(dt);
		++i;
	}
}

void EnemyShip::handle_keyboard_button_down(unsigned char c)
{
	if (c == 'e' || c=='E')
		propulsors->toggle();
}

void EnemyShip::set_collision_point(physx::PxVec3 pos, physx::PxVec3 normal)
{
	if (!dead) {
		dead = true;
		propulsors->set_state(false);

		explosion = new BombGenerator(my_scene, 40, 3, 0.25);//new TriggeredParticleGenerator(my_scene, bomb);
		addChild(explosion);
		//die();
	}
	//std::cout << pos.x << ' ' << pos.y << ' ' << pos.z << " , " << normal.x << ' ' << normal.y << ' ' << normal.z << '\n';
	PxQuat global_pos_to_this_obj_pos = global_transform.q.getConjugate();
	fire_transform f;
	f.pos = global_pos_to_this_obj_pos.rotate(pos - global_transform.p);
	auto pos_normalized = f.pos.getNormalized();
	float angle = physx::PxAcos(pos_normalized.dot({ 0,0,1 }));
	physx::PxVec3 perpendicular = physx::PxVec3(0, 0, 1).cross(pos_normalized).getNormalized();
	f.looking_to = PxQuat(angle, perpendicular);
	auto fire_hit = new ToggleParticleGenerator(my_scene, fire_hit_enemy_ship);
	fire_hit->set_toggle(true);
	addChild(fire_hit);
	points_impacted.push_back(f);

}

bool EnemyShip::alive()
{
	return !to_be_destroyed;
}

float lerp(float a, float b, float c) {
	return a * (1-c) + b * c;
}

Vector3 EnemyShip::think_off_torque() {
	Transform& player_tr = player_go->get_global_tr();
	//This is how i want my forward dir vector to be
	PxVec3 global_vector_to_player = (global_transform.p - player_tr.p).getNormalized();
	//This is how it is right now
	PxVec3 current_dir_vec = global_transform.q.rotate({0,0,1});

	//El cross product me da un vector perpendicular a estos dos
	PxVec3 vector_de_rotación = global_vector_to_player.cross(current_dir_vec);

	//Para obtener que tan rápido debería girar resto estos dos y me da una escala
	float vel_rot = (current_dir_vec - global_vector_to_player).normalize();

	return vel_rot*vector_de_rotación;
}

void EnemyShip::think_step(double dt)
{
	//Aim for the player ship
	rb->addTorque(750 * dt * think_off_torque());
}

#include "EnemyShip.hpp"
#include "SceneObject.hpp"
#include "Distributions.hpp"
#include <iostream>
#include <cmath>
#include <ctgmath>
#include "ParticleGeneratorsDescriptors.hpp"
#include "ParticleGenerator.hpp"
#include "ForceGenerator.hpp"

constexpr float near_threshold_to_flee = 12;

EnemyShip::EnemyShip(physx::PxScene* s, GameObject* player)
	: Rigid_CubeObject(s, config{ Rigidbody_Object::config{SceneObject::config{GameObject::config{},{0,0,0,0}}}, {1,1,1} }, NO_REPRESENTATION::no_representation), player_go(player)
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

	propulsors = new Directional_ForceGenerator(s,{0,0,1}, 10*my_mass.mass);
	addChild(propulsors);
	add_force_to_myself(propulsors);
	//add_force_to_myself("black_hole");
}

void EnemyShip::step(double dt)
{
	think_step(dt);
	global_transform = rb->getGlobalPose();
	integrate(dt);

	int i = 0;
	float x_wing_offset = 1.2f;
	physx::PxVec3 x_axis = global_transform.q.rotate({1,0,0});
	physx::PxVec3 z_axis = global_transform.q.rotate({ 0,0,1 });
	for (auto& child : child_objects) {
		child->step(dt);
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
		case 3: //PITORRO
			Transform aux_tr = global_transform;
			aux_tr.p += z_axis*x_wing_offset;
			child->setTransform(aux_tr);
			break;
		}
		++i;
	}
}

void EnemyShip::handle_keyboard_button_down(unsigned char c)
{
	if (c == 'e' || c=='E')
		propulsors->toggle();
}

/*
physx::PxQuat get_rotation_to(const physx::PxVec3 from, const physx::PxVec3 to) {
	physx::PxQuat q;
	physx::PxVec3 a = from.cross(to);
	q.x = a.x;
	q.y = a.y;
	q.z = a.z;
	auto from_mag = from.magnitude();
	auto to_mag = to.magnitude();
	q.w = (sqrt((from_mag * from_mag * to_mag * to_mag)) + from.dot(to));
	return q;
}
*/

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
	rb->addTorque(/*global_transform.q.rotate(*/1000 * dt * think_off_torque()/*)*/);
	/*
	Transform& player_tr = player_go->get_global_tr();
	PxVec3 global_vector_to_player = global_transform.p - player_tr.p;
	float distance_to_player = (global_vector_to_player - vel).magnitude();
	float rotation_to_apply_in_radians;

	PxVec3 global_ship_dir = global_transform.q.rotate({ 0,0,1 });

	float interpolation_value = 0.005f;
	//find if it's looking towards player
	if ((global_vector_to_player + global_ship_dir).magnitudeSquared() < global_vector_to_player.magnitudeSquared()) {
		interpolation_value = 0.01f;
	}
	//std::cout << interpolation_value << '\n';

	Quaternion q;
	PxVec3 a = global_vector_to_player.cross(global_ship_dir);
	q = Quaternion(a.x,a.y,a.z,
		sqrt((global_vector_to_player.magnitudeSquared()) * (global_ship_dir.magnitudeSquared())) + global_vector_to_player.dot(global_ship_dir));

	//Interpolate current_quat to objective quat
	global_transform.q.x = lerp(global_transform.q.x, q.x, interpolation_value * dt);
	global_transform.q.y = lerp(global_transform.q.y, q.y, interpolation_value * dt);
	global_transform.q.z = lerp(global_transform.q.z, q.z, interpolation_value * dt);
	global_transform.q.w = lerp(global_transform.q.w, q.w, interpolation_value * dt);
	global_transform.q.normalize();
	*/
}

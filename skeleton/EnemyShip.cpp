#include "EnemyShip.hpp"
#include "SceneObject.hpp"
#include "Distributions.hpp"
#include <iostream>
#include <cmath>
#include <ctgmath>
#include "ParticleGeneratorsDescriptors.hpp"
#include "ParticleGenerator.hpp"

constexpr float near_threshold_to_flee = 10;

EnemyShip::EnemyShip(GameObject* player)
	: GameObject(), player_go(player)
{
	addChild(new SphereObject({ SceneObject::config(), 1 }));
	for (auto i = -1; i < 2; i = i + 2) {
		auto cube = new CubeObject({ SceneObject::config(), {0.1, 1.5, 1.5} });
		cube->translate({ i * 1.2f,0,0 });
		addChild(cube);
	}
	SceneObject::config c{};
	c.color = {1,0,0,1};
	auto cube = new CubeObject({ c, {0.1, 0.1, 1} });
	cube->translate({ 0,0,0.5 });
	addChild(cube);
	
	//Set random pos
	translate_to({
		(Distributions::LinearDistribution::get() * 100) - 50,
		(Distributions::LinearDistribution::get() * 100) - 50,
		(Distributions::LinearDistribution::get() * 100) - 50
	});
	set_vel({ 0,0,15});

	ParticleGenerator* paco = new ParticleGenerator(missile_particle_system);
	addChild(paco);
}

void EnemyShip::step(double dt)
{
	think_step(dt);
	GameObject::step(dt);
}

void EnemyShip::update_position(physx::PxTransform const& parent_tr)
{
	parent_to_child_tr = parent_tr;
	GameObject::update_position(parent_tr);
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

void EnemyShip::think_step(double dt)
{
	//Aim for the player ship
	Transform& player_tr = player_go->get_global_tr();
	PxVec3 global_vector_to_player = player_tr.p - global_transform.p;
	PxVec3 local_direction_to_player = global_to_local_rot.rotate(global_vector_to_player);
	float distance_to_player = (local_direction_to_player - vel).magnitude();
	float rotation_to_apply_in_radians;

	PxVec3 local_ship_direction = { 0,0,1 };

	//Si está mirando en dirección hacia el player
	if ((local_direction_to_player + local_ship_direction).z > 0) {
		//rota más rápido para apuntarte mejor y cosas de gameplay y tal
		rotation_to_apply_in_radians = 1*dt;
	}
	else {
		//Rotan más lento para que se alejen más del player al hacer una batida. Y así le vuelve a dar tiempo a apuntarte
		rotation_to_apply_in_radians = 0.33 * dt;
	}

	if (distance_to_player < near_threshold_to_flee) {
		//Si estan muy cerca huyen
		local_direction_to_player *= -1;
	}

	PxVec3 v_orthogonal_to_rotation = local_direction_to_player.cross(local_ship_direction).getNormalized();

	if (PxAbs(1.0f - v_orthogonal_to_rotation.magnitude()) < 1e-3f){
		PxQuat rot_quat = PxQuat(
			-rotation_to_apply_in_radians,
			v_orthogonal_to_rotation.getNormalized()
		);
		rotate(rot_quat);
	}
}

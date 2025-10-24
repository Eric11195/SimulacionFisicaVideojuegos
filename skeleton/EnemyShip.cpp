#include "EnemyShip.hpp"
#include "SceneObject.hpp"
#include "Distributions.hpp"
#include <iostream>
#include <cmath>
#include <ctgmath>
#include "ParticleGeneratorsDescriptors.hpp"
#include "ParticleGenerator.hpp"

constexpr float near_threshold_to_flee = 20;

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
	set_vel({ 0,0,10});

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
	//PREGUNTA AL PROFE => COMO SABER PARA DONDE TENEMOS QUE ROTAR LA NAVE??????????????????????????????????????????????????????????????????????

	//Aim for the player ship
	Transform& player_tr = player_go->get_global_tr();
	PxVec3 vector_to_player = player_tr.p - global_transform.p;
	float distance_to_player = vector_to_player.normalize();
	if (distance_to_player < near_threshold_to_flee) {
		vector_to_player *= -1;
	}
	PxVec3 global_direction_to_player =	global_to_local_rot.rotate(	vector_to_player.getNormalized() );
	//global_direction_to_player = global_direction_to_player;//local_transform.q.rotate(global_direction_to_player).getNormalized();
	PxVec3 global_ship_direction = { 0,0,1 };		//local_transform.q.rotate({ 0,0,1 }).getNormalized();
	PxVec3 v_orthogonal_to_rotation = global_direction_to_player.cross(global_ship_direction).getNormalized();
	//SACAR SI LA X DE ESTE VECTOR EN EL SISTEMA DE COORDENADAS LOCAL APUNTA HACIA LA DERECHA O HACIA LA IZQUIERA
	float rotation_to_apply_in_radians = 0.5 * dt;

	if (PxAbs(1.0f - v_orthogonal_to_rotation.magnitude()) < 1e-3f){
		PxQuat rot_quat = PxQuat(
			-rotation_to_apply_in_radians,// * (rotation_from_current_rot_to_player_pointing_rot > 0 ? 1 : -1), 
			v_orthogonal_to_rotation.getNormalized()
		);
		//std::cout << rot_quat.x <<" "<< rot_quat.y << " " << rot_quat.z << " " << rot_quat.w << '\n';
		rotate(rot_quat);
		//rotate(perfect_rotation);
	}
}

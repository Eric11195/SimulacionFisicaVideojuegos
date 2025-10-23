#include "EnemyShip.hpp"
#include "SceneObject.hpp"
#include "Distributions.hpp"
#include <iostream>
#include <cmath>
#include <ctgmath>

EnemyShip::EnemyShip(GameObject* player)
	: GameObject(), player_go(player)
{
	addChild(new SphereObject({ SceneObject::config(), 1 }));
	for (auto i = -1; i < 2; i = i + 2) {
		auto cube = new CubeObject({ SceneObject::config(), {0.1, 1.5, 1.5} });
		cube->translate({ i * 1.2f,0,0 });
		addChild(cube);
	}
	//Set random pos
	translate_to({
		(Distributions::LinearDistribution::get() * 50) - 25,
		(Distributions::LinearDistribution::get() * 50) - 25,
		(Distributions::LinearDistribution::get() * 50) - 25
	});
	set_vel({ 0,0,10});
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

void EnemyShip::think_step(double dt)
{
	//Aim for the player ship
	Transform& player_tr = player_go->get_global_tr();
	PxVec3 global_direction_to_player = (global_transform.p - player_tr.p).getNormalized();
	//global_direction_to_player = global_direction_to_player;//local_transform.q.rotate(global_direction_to_player).getNormalized();
	PxVec3 global_ship_direction = local_transform.q.rotate({ 0,0,1 }).getNormalized();
	PxVec3 v_orthogonal_to_rotation = global_direction_to_player.cross(global_ship_direction).getNormalized();
	float rotation_to_apply_in_radians = 0.5 * dt;
	float arccos_of_angle_to_player_pos_rot = global_direction_to_player.dot(global_ship_direction);
	float rotation_from_current_rot_to_player_pointing_rot = acos(arccos_of_angle_to_player_pos_rot);
	std::cout << arccos_of_angle_to_player_pos_rot <<" rot " << rotation_from_current_rot_to_player_pointing_rot << '\n';// << "  vec " << v_orthogonal_to_rotation.x << " " << v_orthogonal_to_rotation.y << " " << v_orthogonal_to_rotation.z << '\n';
	
	//std::cout << rotation_to_apply_in_radians * (rotation_from_current_rot_to_player_pointing_rot > 0 ? 1 : -1) <<"\n";
	if (PxAbs(1.0f - v_orthogonal_to_rotation.magnitude()) < 1e-3f){
		PxQuat rot_quat = PxQuat(
			rotation_to_apply_in_radians * (rotation_from_current_rot_to_player_pointing_rot > 0 ? 1 : -1), 
			v_orthogonal_to_rotation.getNormalized()
		);
		rotate(rot_quat);
	}

/* Perfect aim
	PxQuat perfect_rotation_to_player = get_rotation_to(global_direction_to_player, global_ship_direction).getNormalized();
	rotate(perfect_rotation_to_player);
*/
	//Poner que rote una cantidad por delta time en esa dirección
	//Shoot
}

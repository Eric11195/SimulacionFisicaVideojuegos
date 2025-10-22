#include "EnemyShip.hpp"
#include "SceneObject.hpp"
#include "Distributions.hpp"

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
		Distributions::LinearDistribution::get() * 50 - 25,
		Distributions::LinearDistribution::get() * 50 - 25,
		Distributions::LinearDistribution::get() * 50 - 25
	});
	set_vel({ 1,0,0 });
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
	PxVec3 global_direction_to_player = global_transform.p - player_tr.p;
	PxVec3 global_ship_direction = global_transform.q.rotate({ 0,0,1 });
	PxQuat perfect_rotation_to_player = get_rotation_to(global_direction_to_player, global_ship_direction).getNormalized();
	rotate(perfect_rotation_to_player);
	//Rotamos primero con la inversa de su rotación global y luego con la de padre a hijo
	
	//THIS STINKS => I need the 
	/*
	//WORKS ONLY ASSUMING SCENE_GAMEOBJECT IS ITS PARENT
	auto local_direction_to_player = global_to_local_rot.rotate(direction_to_player).getNormalized();//parent_to_child_tr.q.rotate(global_to_local_rot.rotate(direction_to_player)).getNormalized();
	PxQuat rot_to_player = get_rotation_to(local_direction_to_player, ).getNormalized();
	PxQuat rot_to_player_dir();
	rotate(rot_to_player);
	*/
	

	//Poner que rote una cantidad por delta time en esa dirección
	//Shoot
}

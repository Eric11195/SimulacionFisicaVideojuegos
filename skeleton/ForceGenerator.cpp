#include "ForceGenerator.hpp"


Plain_Directional_ForceGenerator::Plain_Directional_ForceGenerator(std::string name, physx::PxVec3 force_direction, float force_magnitude)
	:ForceGenerator(name, force_direction, force_magnitude) {}


//F = masa * accel
physx::PxVec3 Plain_Directional_ForceGenerator::apply_force(GameObject const& g)
{
	return global_force;
}

void ForceGenerator::update_position(Transform const& parent_tr)
{
	GameObject::update_position(parent_tr);
	global_force = parent_tr.rotate(local_force);
}

ForceGenerator::ForceGenerator(std::string name, physx::PxVec3 force_direction, float magnitude)
	:my_name(name)
{
	auto it = GameObject::force_generators_map.find(name);
	if (it != GameObject::force_generators_map.end()) {
		throw "There's already a force generator with that name";
	}
	GameObject::force_generators_map.emplace(name, this);

	auto force = force_direction.getNormalized() * magnitude;
	local_force = force;
	global_force = force;
}

/*
ForceGenerator::~ForceGenerator()
{
	//Si esto se está llamando es pq ya nadie tiene referencias a él
	auto it = GameObject::force_generators_map.find(my_name);
	if (it == GameObject::force_generators_map.end()) {
		throw "There is no force generator with that name. Probably it has already been deleted";
	}
	GameObject::force_generators_map.erase(it);
	
}
*/
Gravity_ForceGenerator::Gravity_ForceGenerator(physx::PxVec3 v)
	: ForceGenerator("gravity", v, 0.98f) {}

//returns the force to give the given object
physx::PxVec3 Gravity_ForceGenerator::apply_force(GameObject const& g)
{
	auto mass = g.get_inv_mass();
	if (mass < 0.005f) return { 0,0,0 };
	return global_force * mass;
}

Wind_ForceGenerator::Wind_ForceGenerator(std::string s, physx::PxVec3 v, float magnitude, float air_density, float avance_resistance_aerodinamic_coef)
	: ForceGenerator(s, v, magnitude), cd_p_medios(0.5*air_density*avance_resistance_aerodinamic_coef)
{
}

physx::PxVec3 Wind_ForceGenerator::apply_force(GameObject const& g)
{
	//DIAPO 25 forma sencilla
	//velocidad al cuadrado => vector velocidad por su modulo
	return cd_p_medios * global_force * global_force.magnitude();
	//DIAPO 26 => No entiendo que tiene que ver la velocidad del objeto para calcular la fuerza
	/*
	physx::PxVec3 vel_delta = g.get_vel() - global_force;
	float module = vel_delta.magnitude();
	return cd_p_medios * module * vel_delta;// *g.get_inv_mass();
	*/
}

Torbellino_ForceGenerator::Torbellino_ForceGenerator(std::string s, physx::PxVec3 v, float magnitude, float air_density, float avance_resistance_aerodinamic_coef, axis_lock l)
	:Wind_ForceGenerator(s,v,magnitude, air_density, avance_resistance_aerodinamic_coef), my_axis_locked(l){ }

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

physx::PxVec3 Torbellino_ForceGenerator::apply_force(GameObject const& g)
{
	if (!inside_area_of_influence(g)) return { 0,0,0 };

	//Make the rotation and thingies
	Transform const& player_tr = g.get_global_tr();
	physx::PxVec3 global_vector_to_player = player_tr.p - global_transform.p;
	//Lo normaliza y cojemos el modulo
	float distance_to_object = global_vector_to_player.normalize();
	
	physx::PxVec3 local_direction = global_transform.q.rotate({1,0,0});
	physx::PxQuat rotation = get_rotation_to(global_vector_to_player,local_direction).getNormalized();

	physx::PxVec3 force_applied = Wind_ForceGenerator::apply_force(g);
	force_applied = rotation.rotate(force_applied);
	//float force_applied_mag = force_applied.magnitude();

	//Lock axis
	//If an axis is locked, there will be no force applied to it
	//Axis are relative to this force generator
	force_applied.x *= min(1, my_axis_locked & axis_lock::free_x);
	force_applied.y *= min(1, my_axis_locked & axis_lock::free_y);
	force_applied.z *= min(1, my_axis_locked & axis_lock::free_z);

	//Normalizar para que sea la misma fuerza la aplicada aún con el axis lock
	force_applied = force_applied.getNormalized() * distance_to_object;

	return force_applied;
}

bool Torbellino_ForceGenerator::inside_area_of_influence(GameObject const& g) const
{
	return true;
}

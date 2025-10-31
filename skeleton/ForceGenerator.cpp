#include "ForceGenerator.hpp"
#include <iostream>


Directional_ForceGenerator::Directional_ForceGenerator(physx::PxVec3 force_direction, float force_magnitude)
	:ForceGenerator(force_magnitude), normalized_force_direction(force_direction.getNormalized())
{
}

Directional_ForceGenerator::Directional_ForceGenerator(std::string name, physx::PxVec3 force_direction, float force_magnitude)
	:ForceGenerator(name, force_magnitude), normalized_force_direction(force_direction.getNormalized()) {}


//F = masa * accel
physx::PxVec3 Directional_ForceGenerator::apply_force(GameObject const& g)
{
	return global_transform.q.rotate(force_magnitude*normalized_force_direction);
}

ForceGenerator::ForceGenerator(float force_magnitude)
	:force_magnitude(force_magnitude)
{
}

ForceGenerator::ForceGenerator(std::string name, float magnitude)
	:force_magnitude(magnitude), my_name(name)
{
	auto it = GameObject::force_generators_map.find(name);
	if (it != GameObject::force_generators_map.end()) {
		throw "There's already a force generator with that name";
	}
	GameObject::force_generators_map.emplace(name, this);
}

void ForceGenerator::cleanup_me()
{
	if (my_name == "-1") return;

	auto it = GameObject::force_generators_map.find(my_name);
	if (it == GameObject::force_generators_map.end()) {
		throw "There is no force generator with that name. Probably it has already been deleted";
	}
	GameObject::force_generators_map.erase(it);
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
Gravity_ForceGenerator::Gravity_ForceGenerator(physx::PxVec3 v, float mag)
	: Directional_ForceGenerator(v, mag) {}

Gravity_ForceGenerator::Gravity_ForceGenerator(std::string name, physx::PxVec3 force_direction, float mag)
	: Directional_ForceGenerator(name, force_direction, mag)
{
}

//returns the force to give the given object
physx::PxVec3 Gravity_ForceGenerator::apply_force(GameObject const& g)
{
	auto inv_mass = g.get_inv_mass();
	if (inv_mass < 0.005f) return { 0,0,0 };
	return Directional_ForceGenerator::apply_force(g) / inv_mass;
}

Wind_ForceGenerator::Wind_ForceGenerator(physx::PxVec3 v, float magnitude, float air_density, float avance_resistance_aerodinamic_coef)
	:Directional_ForceGenerator(v, magnitude), cd_p_medios(0.5 * air_density * avance_resistance_aerodinamic_coef)
{
}

Wind_ForceGenerator::Wind_ForceGenerator(std::string s, physx::PxVec3 v, float magnitude, float air_density, float avance_resistance_aerodinamic_coef)
	: Directional_ForceGenerator(s, v, magnitude), cd_p_medios(0.5*air_density*avance_resistance_aerodinamic_coef)
{
}

physx::PxVec3 Wind_ForceGenerator::apply_force(GameObject const& g)
{
	//DIAPO 25 forma sencilla
	auto force = Directional_ForceGenerator::apply_force(g);
	//velocidad al cuadrado => vector velocidad por su modulo
	return cd_p_medios * force * force.magnitude();
	//DIAPO 26 => No entiendo que tiene que ver la velocidad del objeto para calcular la fuerza
	/*
	physx::PxVec3 vel_delta = g.get_vel() - global_force;
	float module = vel_delta.magnitude();
	return cd_p_medios * module * vel_delta;// *g.get_inv_mass();
	*/
}

constexpr float k1 = 1, k2 = 0;
physx::PxVec3 Wind_ForceGenerator::calculate_force(physx::PxVec3 wind_speed, physx::PxVec3 obj_speed)
{
	physx::PxVec3 diff_vec = wind_speed - obj_speed;
	return k1 * diff_vec + k2 * diff_vec.magnitude()*diff_vec;
}

Torbellino_ForceGenerator::Torbellino_ForceGenerator(physx::PxVec3 pos, float magnitude, float air_density, float avance_resistance_aerodinamic_coef, axis_lock l)
	: Wind_ForceGenerator(physx::PxVec3(0,0,0), magnitude, air_density, avance_resistance_aerodinamic_coef), my_axis_locked(l)
{
	global_transform.p = pos;
}

Torbellino_ForceGenerator::Torbellino_ForceGenerator(std::string s, physx::PxVec3 pos, float magnitude, float air_density, float avance_resistance_aerodinamic_coef, axis_lock l)
	:Wind_ForceGenerator(s,Vector3(0,0,1), magnitude, air_density, avance_resistance_aerodinamic_coef), my_axis_locked(l) 
{
	global_transform.p = pos;
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

physx::PxVec3 Torbellino_ForceGenerator::apply_force(GameObject const& g)
{
	if (!inside_area_of_influence(g)) return { 0,0,0 };

	//Make the rotation and thingies
	Transform const& player_tr = g.get_global_tr();
	physx::PxVec3 global_vector_to_player = (player_tr.p - global_transform.p).getNormalized();
	//Lo normaliza y cojemos el modulo
	
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
	force_applied = force_applied.getNormalized();// * distance_to_object;
	//std::cout << force_applied.x << ' ' << force_applied.y << ' ' << force_applied.z << '\n';

	return force_applied;
}

bool Torbellino_ForceGenerator::inside_area_of_influence(GameObject const& g) const
{
	return true;
}

TorbellinoSencillo::TorbellinoSencillo(std::string s, physx::PxVec3 v, float magnitude, float air_density, float avance_resistance_aerodinamic_coef)
	:Wind_ForceGenerator(s,v,magnitude,air_density,avance_resistance_aerodinamic_coef)
{
}

physx::PxVec3 TorbellinoSencillo::apply_force(GameObject const& g)
{
	if (!inside_area_of_influence(g)) return { 0,0,0 };

	physx::PxVec3& g_pos = g.get_global_tr().p;
	physx::PxVec3 vel_torbellino =  10 * physx::PxVec3{
				-(g_pos.z - global_transform.p.z),
		 50 - (g_pos.y - global_transform.p.y),
				(g_pos.x - global_transform.p.x)
	};
	auto wind_force = calculate_force(vel_torbellino, g.get_vel());
	//std::cout << g_pos.x << " " << (50-g_pos.y) << " " << g_pos.z << " - " << global_transform.p.x << " " << global_transform.p.y << ' ' << global_transform.p.z << " --> " << vel_torbellino.x << ' ' << vel_torbellino.y << ' ' << vel_torbellino.z << '\n';

	return wind_force;
}

bool TorbellinoSencillo::inside_area_of_influence(GameObject const& g) const
{
	return true;
}

Variable_ForceGenerator::Variable_ForceGenerator(float force_magnitude,
	std::function<physx::PxVec3(float force, GameObject const& self, GameObject const& g)> force_function,
	std::function<void(double time, double dt, float& force)> force_update_func)
	: ForceGenerator(force_magnitude), force_value_func(force_function), update_force_func(force_update_func), time_since_started(0) { }

Variable_ForceGenerator::Variable_ForceGenerator(std::string s, float force_magnitude,
	std::function<physx::PxVec3(float force_mag, GameObject const& self, GameObject const& g)> force_function,
	std::function <void(double time,double dt, float& force)> force_update_func)
	: ForceGenerator(s,force_magnitude), force_value_func(force_function), update_force_func(force_update_func),time_since_started(0) { }

physx::PxVec3 Variable_ForceGenerator::apply_force(GameObject const& g)
{
	return force_value_func(force_magnitude,*this, g);
}

void Variable_ForceGenerator::step(double dt)
{
	ForceGenerator::step(dt);
	time_since_started += dt;
	update_force_func(time_since_started, dt, force_magnitude);
}

ToggleDirectional_ForceGenerator::ToggleDirectional_ForceGenerator(physx::PxVec3 force_dir, float force_mag, bool start_state)
	:Directional_ForceGenerator(force_dir, force_mag), active(start_state)
{
}

physx::PxVec3 ToggleDirectional_ForceGenerator::apply_force(GameObject const& g)
{
	if (active) return Directional_ForceGenerator::apply_force(g);

	return physx::PxVec3(0,0,0);
}

void ToggleDirectional_ForceGenerator::set_state(bool state)
{
	active = state;
}

#include "ForceGenerator.hpp"


Directional_ForceGenerator::Directional_ForceGenerator(std::string name, physx::PxVec3 force_direction, float force_magnitude) 
	:ForceGenerator(name) {
	auto force = force_direction.getNormalized() * force_magnitude;
	local_force = force;
	global_force = force;
}


//F = masa * accel
physx::PxVec3 Directional_ForceGenerator::apply_force(GameObject const& g)
{
	return global_force;
}

void Directional_ForceGenerator::update_position(Transform const& parent_tr)
{
	GameObject::update_position(parent_tr);
	global_force = parent_tr.rotate(local_force);
}

ForceGenerator::ForceGenerator(std::string name)
	:my_name(name)
{
	auto it = GameObject::force_generators_map.find(name);
	if (it != GameObject::force_generators_map.end()) {
		throw "There's already a force generator with that name";
	}
	GameObject::force_generators_map.emplace(name, this);
}

ForceGenerator::~ForceGenerator()
{
	auto it = GameObject::force_generators_map.find(my_name);
	if (it == GameObject::force_generators_map.end()) {
		throw "There is no force generator with that name. Probably it has already been deleted";
	}
}
Gravity_ForceGenerator::Gravity_ForceGenerator(physx::PxVec3 v)
	: Directional_ForceGenerator("gravity", v, 0.98f) {}

//returns the force to give the given object
physx::PxVec3 Gravity_ForceGenerator::apply_force(GameObject const& g)
{
	auto mass = g.get_inv_mass();
	if (mass < 0.005f) return { 0,0,0 };
	return global_force * mass;
}

Wind_ForceGenerator::Wind_ForceGenerator(std::string s, physx::PxVec3 v, float magnitude, float air_density, float avance_resistance_aerodinamic_coef)
	:Directional_ForceGenerator(s, v, magnitude), cd_p_medios(0.5*air_density*avance_resistance_aerodinamic_coef)
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

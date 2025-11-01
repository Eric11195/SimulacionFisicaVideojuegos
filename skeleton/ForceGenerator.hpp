#pragma once
#include "GameObject.hpp"
#include <bitset>
#include <functional>

class ForceGenerator : public GameObject {
public:
	ForceGenerator(float force_magnitude);
	ForceGenerator(std::string name, float force_magnitude);
	virtual physx::PxVec3 apply_force(GameObject const& g) = 0;
	void cleanup_me();
protected:
	float force_magnitude;
	std::string my_name = "-1";
};

//For performance issues this should be changed to saving only once each frame the global force applied
class Directional_ForceGenerator : public ForceGenerator {
public:
	Directional_ForceGenerator(physx::PxVec3 force_direction, float force_magnitude);
	Directional_ForceGenerator(std::string s, physx::PxVec3 force_direction, float force_magnitude);
	virtual physx::PxVec3 apply_force(GameObject const& g) override;
protected:
	physx::PxVec3 normalized_force_direction;
};

class ToggleDirectional_ForceGenerator : public Directional_ForceGenerator {
public:
	ToggleDirectional_ForceGenerator(physx::PxVec3 force_dir, float force_mag, bool start_state);
	virtual physx::PxVec3 apply_force(GameObject const& g) override;
	void set_state(bool state);
protected:
	bool active;
};

class Gravity_ForceGenerator : public Directional_ForceGenerator {
public:
	Gravity_ForceGenerator(physx::PxVec3 force_dir, float mag = 9.8f);
	Gravity_ForceGenerator(std::string name, physx::PxVec3 force_direction, float mag = 9.8f);
	virtual physx::PxVec3 apply_force(GameObject const& g) override;
};

//standard sea level density of air at 101.325 kPa(abs) and 15 �C(59 �F) is 1.2250 kg / m3
class Wind_ForceGenerator : public Directional_ForceGenerator {
public:
	Wind_ForceGenerator(physx::PxVec3, float magnitude, float air_density = 1.33, float avance_resistance_aerodinamic_coef = 0.5f);
	Wind_ForceGenerator(std::string s, physx::PxVec3, float magnitude, float air_density=1.33, float avance_resistance_aerodinamic_coef=0.5f);
	virtual physx::PxVec3 apply_force(GameObject const& g) override;
protected:
	physx::PxVec3 calculate_force(physx::PxVec3 wind_speed, physx::PxVec3 obj_speed);
	//El valor calculado en el constructor constante por el que se multiplicar� la fuerza
	const float cd_p_medios;
};

class TorbellinoSencillo : public Wind_ForceGenerator {
public:
	TorbellinoSencillo(std::string s, physx::PxVec3, float magnitude, float air_density=1.33, float avance_resistance_aerodinamic_coef=0.5f);
	virtual physx::PxVec3 apply_force(GameObject const& g) override;
protected:
	virtual bool inside_area_of_influence(GameObject const& g) const;
};

//This type of generator always owns the particles or objects it manages. For the case in which it erases itself, so no references are left
class Variable_ForceGenerator : public ForceGenerator {
public:
	Variable_ForceGenerator(float force_magnitude,
		std::function<physx::PxVec3(float force, float time, GameObject const& self, GameObject const& g)> force_function
	);
	Variable_ForceGenerator(std::string s, float force_magnitude, 
		std::function<physx::PxVec3(float force, float time, GameObject const& self, GameObject const& g)> force_function
	);
	virtual physx::PxVec3 apply_force(GameObject const& g) override;
	virtual void step(double dt) override;
protected:
	std::function<physx::PxVec3(float force, float time, GameObject const& self, GameObject const& g)> force_value_func;

	double time_since_started;
};
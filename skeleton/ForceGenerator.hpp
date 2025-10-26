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

class Gravity_ForceGenerator : public Directional_ForceGenerator {
public:
	Gravity_ForceGenerator(physx::PxVec3 force_dir);
	Gravity_ForceGenerator(std::string name, physx::PxVec3 force_direction);
	virtual physx::PxVec3 apply_force(GameObject const& g) override;
};

//standard sea level density of air at 101.325 kPa(abs) and 15 °C(59 °F) is 1.2250 kg / m3
class Wind_ForceGenerator : public Directional_ForceGenerator {
public:
	Wind_ForceGenerator(physx::PxVec3, float magnitude, float air_density = 1.33, float avance_resistance_aerodinamic_coef = 0.5f);
	Wind_ForceGenerator(std::string s, physx::PxVec3, float magnitude, float air_density=1.33, float avance_resistance_aerodinamic_coef=0.5f);
	virtual physx::PxVec3 apply_force(GameObject const& g) override;
protected:
	//El valor calculado en el constructor constante por el que se multiplicará la fuerza
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
		std::function<physx::PxVec3(float force, GameObject const& self, GameObject const& g)> force_function,
		std::function <void(double time, double dt, float& force)> = std::function<void(double time, double dt, float& force)>([=](double a, double c, float& b) {})
	);
	Variable_ForceGenerator(std::string s, float force_magnitude, 
		std::function<physx::PxVec3(float force, GameObject const& self, GameObject const& g)> force_function,
		std::function <void(double time,double dt, float& force)> = std::function<void(double time,double dt, float& force)>([=](double a,double c, float& b) { })
	);
	virtual physx::PxVec3 apply_force(GameObject const& g) override;
	virtual void step(double dt) override;
protected:
	std::function<physx::PxVec3(float force,GameObject const& self, GameObject const& g)> force_value_func;
	std::function <void(double time,double dt, float& force)> update_force_func;
	double time_since_started;
};


//TORBELLINO
//MISMAS ECUACIONES QUE EL VIENTO, pero la dirección de la fuerza es perpendicular al vector que une al objeto con el generador
//Tiene una dirección, que es => en el punto 1,0,0 en que dirección iría la partícula y la magnitud de esa fuerza
//Solo tengo que rotar esa fuerza, con el quaternion de pasar de 1,0,0 a la dirección normalizada del vector que une el objeto con el generador
//O sea lo mismo que se hace ahora mismo con las naves
//Tiene como opción bloquear alguno de los ejes. De forma que solo dependa de los otros dos, o de uno. 
//Al hacer esto en el vector que une el objeto con la fuerza. Las componentes de los ejes correspondientes se multiplicarán por cero
class Torbellino_ForceGenerator : public Wind_ForceGenerator {
public:
	//Locking 2 axis in theory would make no sense (creo)
	enum axis_lock {
		none = 07,
		x = 06,
		y = 05,
		z = 03,
		free_x = 01,
		free_y = 02,
		free_z = 04,
		all = 00
	};
	Torbellino_ForceGenerator(physx::PxVec3, float magnitude, float air_density, float avance_resistance_aerodinamic_coef, axis_lock l);
	Torbellino_ForceGenerator(std::string s, physx::PxVec3, float magnitude, float air_density, float avance_resistance_aerodinamic_coef, axis_lock l);
	virtual physx::PxVec3 apply_force(GameObject const& g) override;
protected:
	axis_lock my_axis_locked;
	virtual bool inside_area_of_influence(GameObject const& g) const;
};

#pragma once
#include "GameObject.hpp"
#include <bitset>

class ForceGenerator : public GameObject {
public:
	ForceGenerator(std::string name, physx::PxVec3 force_direction, float force_magnitude);
	//~ForceGenerator();
	virtual physx::PxVec3 apply_force(GameObject const& g) = 0;
	virtual void update_position(Transform const& parent_tr) override;
protected:
	physx::PxVec3 local_force;
	physx::PxVec3 global_force;
	std::string my_name;
};

class Plain_Directional_ForceGenerator : public ForceGenerator {
public:
	Plain_Directional_ForceGenerator(std::string s, physx::PxVec3 force_direction, float force_magnitude);
	virtual physx::PxVec3 apply_force(GameObject const& g) override;
};

class Gravity_ForceGenerator : public ForceGenerator {
public:
	Gravity_ForceGenerator(physx::PxVec3 force_direction);
	virtual physx::PxVec3 apply_force(GameObject const& g) override;
};


class Wind_ForceGenerator : public ForceGenerator {
public:
	Wind_ForceGenerator(std::string s, physx::PxVec3, float magnitude, float air_density, float avance_resistance_aerodinamic_coef);
	virtual physx::PxVec3 apply_force(GameObject const& g) override;
protected:
	//El valor calculado en el constructor constante por el que se multiplicará la fuerza
	const float cd_p_medios;
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
	Torbellino_ForceGenerator(std::string s, physx::PxVec3, float magnitude, float air_density, float avance_resistance_aerodinamic_coef, axis_lock l);
	virtual physx::PxVec3 apply_force(GameObject const& g) override;
protected:
	axis_lock my_axis_locked;
	virtual bool inside_area_of_influence(GameObject const& g) const;
};
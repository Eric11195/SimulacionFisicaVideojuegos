#pragma once

#include "SceneObject.hpp"

class Rigidbody_Object : public SceneObject {
public:
	struct config {
		SceneObject::config so_config;

	};
	Rigidbody_Object(PxScene* s, PxShape* sh,config c);
protected:
	PxRigidDynamic* rb;
};
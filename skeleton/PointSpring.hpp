#pragma once
#include "core.hpp"
#include "ForceGenerator.hpp"
#include "SceneObject.hpp"

class PointSpring : public PT_OBJ_Spring_ForceGenerator{
public:
	PointSpring(physx::PxVec3 v, config c, std::string name);
};
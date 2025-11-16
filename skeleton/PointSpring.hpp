#pragma once
#include "core.hpp"
#include "ForceGenerator.hpp"


class PointSpring : public PT_OBJ_Spring_ForceGenerator{
public:
	PointSpring(physx::PxVec3 v, config c, std::string name);
};
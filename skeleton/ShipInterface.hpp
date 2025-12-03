#pragma once
#include <functional>
#include "core.hpp"

class ShipInterface {
public:
	virtual void die();
	virtual void set_collision_point(physx::PxVec3 pos, physx::PxVec3 normal) {}
	void assign_die_func(std::function<void()> d);
private:
	std::function<void()> die_func = []() {};
};
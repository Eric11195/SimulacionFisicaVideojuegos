#include "ShipInterface.hpp"

void ShipInterface::die()
{
	die_func();
}

void ShipInterface::assign_die_func(std::function<void()> d)
{
	die_func = d;
}

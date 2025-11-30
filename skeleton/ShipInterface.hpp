#pragma once
#include <functional>

class ShipInterface {
public:
	virtual void die();
	void assign_die_func(std::function<void()> d);
private:
	std::function<void()> die_func = []() {};
};
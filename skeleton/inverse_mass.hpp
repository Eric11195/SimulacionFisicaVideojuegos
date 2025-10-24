#pragma once
#include <limits>

enum zero_mass{
	zero,
};
enum infinite_mass {
	infinite
};

struct InvMass {
	float inv_mass;
	InvMass(float inv_mass)
		:inv_mass(inv_mass){ }
	InvMass(zero_mass)
		:inv_mass(std::numeric_limits<float>::infinity()) {}
	InvMass(infinite_mass)
		:inv_mass(0) {}
};
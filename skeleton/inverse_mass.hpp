#pragma once
#include <limits>

enum zero_mass{
	zero,
};
enum infinite_mass {
	infinite
};

struct Mass {
	float mass;
	Mass(float d)
		:mass(d) {}
};

struct InvMass {
	float inv_mass;
	InvMass(float inv_mass)
		:inv_mass(inv_mass){ }
	InvMass(zero_mass)
		:inv_mass(std::numeric_limits<float>::infinity()) {}
	InvMass(infinite_mass)
		:inv_mass(0) {}
	InvMass(Mass m):inv_mass(1) {
		if (m.mass < 0.0001f)inv_mass = InvMass(zero_mass::zero).inv_mass;
		else inv_mass /= m.mass;
	}
};
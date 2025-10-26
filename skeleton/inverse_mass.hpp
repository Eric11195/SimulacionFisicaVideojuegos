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
		:mass(d) {
		if (d < 0)throw "La masa no puede ser negativa";
	}
	Mass& operator +=(Mass const& m1) {
		mass = max(0,m1.mass+mass);
		return *this;
	}
	friend Mass operator +(Mass m1, Mass const& m2) {
		m1 += m2;
		return m1;
	}
};

struct InvMass {
	float inv_mass;
	InvMass(float inv_mass)
		:inv_mass(inv_mass) {
		if (inv_mass < 0) throw "La masa no puede ser negativa";
	}
	//Masa muy alta es un millon por el momento
	InvMass(zero_mass)
		:inv_mass(1000000) {}
	InvMass(infinite_mass)
		:inv_mass(0) {}
	InvMass(Mass m):inv_mass(1) {
		if (m.mass < 0.00000001f)inv_mass = InvMass(zero_mass::zero).inv_mass;
		else inv_mass /= m.mass;
	}
};
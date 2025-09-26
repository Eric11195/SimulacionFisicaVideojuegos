#pragma once
#include "GameObject.hpp"

class CoordinateAxis {
private:
	GameObject* go[4];
public:
	CoordinateAxis() {
		go[0] = new SphereObject(1);
		go[1] = new SphereObject(1, My_Vector3(5, 0, 0), Color(1, 0, 0, 1));
		go[2] = new SphereObject(1, My_Vector3(0, 5, 0), Color(0, 1, 0, 1));
		go[3] = new SphereObject(1, My_Vector3(0, 0, 5), Color(0, 0, 1, 1));
	}
	~CoordinateAxis() {
		for (int i = 0; i < 4; ++i) {
			go[i]->cleanup();
		}
	}
};
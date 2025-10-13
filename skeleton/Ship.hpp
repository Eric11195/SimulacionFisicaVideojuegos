#pragma once
#include "CompositeGameObject.hpp"

//class PxVec3;

class Ship : public CompositeGameObject {
public:
	Ship();
	//virtual void process_input(unsigned char c) override;
	virtual void step(double dt) override;
	virtual void handle_keyboard_button_down(unsigned char c) override;
	virtual void handle_keyboard_button_up(unsigned char c) override;
	virtual void handle_mouse_pos(int x, int y) override;
protected:
	struct angular_velocity {
		float angle;
		physx::PxVec3 rotation_axis;
	};
	angular_velocity current_angular_velocity = { 0,{0,0,1} };
};
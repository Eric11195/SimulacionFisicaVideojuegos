#pragma once
#include "GameObject.hpp"

class Ship : public GameObject {
public:
	Ship();
	//virtual void process_input(unsigned char c) override;
	virtual void step(double dt) override;
	virtual void update_position(Transform const& tr) override;
	virtual void handle_keyboard_button_down(unsigned char c) override;
	virtual void handle_keyboard_button_up(unsigned char c) override;
	virtual void handle_mouse_pos(float x, float y) override;
	virtual void handle_mouse_button_down(uint8_t) override;
	virtual void handle_mouse_button_up(uint8_t) override;
protected:
	struct angular_velocity {
		float angle;
		physx::PxVec3 rotation_axis;
	};
	angular_velocity current_angular_velocity = { 0,{0,0,1} };
	physx::PxVec2 virar_buttons = {0,0};
	//from 0 to 1
	float speed = 0;
	float angular_speed_radians_per_second = 1;
	float virar_radians_per_second = 1.5f;
	enum state {
		constante = 0,
		acelerando = 1,
		decelerando = -1
	};
	state current_state = constante;
};
#include "Ship.hpp"
#include "ShipCannon.hpp"
#include <iostream>
#include <cmath>
#include "utils.hpp"
#include "ScreenSizeConstants.hpp"
#include "ParticleGeneratorsDescriptors.hpp"

Ship::Ship()
	:GameObject()
{
	addChild(new ShipCannon(global_transform));
}

constexpr int max_speed = 3;
void Ship::step(double dt)
{
	GameObject::step(dt);

	//Girar
	if(PxAbs(1.0f - current_angular_velocity.rotation_axis.magnitude()) < 1e-3f)
		rotate(PxQuat(dt*current_angular_velocity.angle*angular_speed_radians_per_second, current_angular_velocity.rotation_axis));

	//Virar
	float virar_radians_vel = -virar_radians_per_second * dt * (virar_buttons[0] - virar_buttons[1]);
	rotate(PxQuat(virar_radians_vel, PxVec3(0, 0, 1)));
	
	//advance
	if (current_state != constante) {
		float desired_speed = min(1, max(0, int(current_state)));
		speed += dt * (desired_speed - speed);
		set_vel({ 0,0,max_speed * speed });
	}

}

void Ship::update_position(Transform const& tr)
{
	GameObject::update_position(tr);
	GetCamera()->setTransform(global_transform);
}

void Ship::handle_keyboard_button_down(unsigned char c)
{
	switch (c) {
	case ' ':
		break;
	case 'a':
		virar_buttons[0] = 1;
		break;
	case 'd':
		virar_buttons[1] = 1;
		break;
	case'w':
		current_state = state(current_state+1);
		break;
	case 's':
		current_state = state(current_state - 1);
		break;
	}

}

void Ship::handle_keyboard_button_up(unsigned char c)
{

	switch (c) {
	case ' ':
		static_cast<ShipCannon*>(child_objects.front().get())->fire_missile();
		//static_cast<Missile*>((*(++child_objects.begin())).get())->trigger();
		break;
	case 'a':
		virar_buttons[0] = 0;
		break;
	case 'd':
		virar_buttons[1] = 0;
		break;
	case'w':
		current_state = state(current_state - 1);
		break;
	case 's':
		current_state = state(current_state + 1);
		break;
	}

}


constexpr int dead_zone = 0.0005;
float max_rot_val = 0.75;
//constexpr float max_rot;
void Ship::handle_mouse_pos(float x, float y)
{
	//ESTO SOLO TIENE EN CUENTA PARA CUANDO X > 0.5

	float x_m1_1_val = 2*(x-0.5);
	x_m1_1_val = (x_m1_1_val < 0 ? -1 : 1) * min(1, abs(x_m1_1_val / max_rot_val));
	float y_m1_1_val = 2 * (y - 0.5);
	y_m1_1_val = (y_m1_1_val < 0 ? -1 : 1) * min(1, abs(y_m1_1_val / max_rot_val));

	//std::lerp
	PxVec3 normalized_rot_direction = PxVec3(y_m1_1_val,-x_m1_1_val, 0);
	const float magnitude = normalized_rot_direction.normalize();
	current_angular_velocity = { 3.14f * min(magnitude,1) , normalized_rot_direction };
}

void Ship::handle_mouse_button_down(uint8_t but)
{
	switch (but) {
	case mouse_button_id::m1:
		static_cast<ShipCannon*>(child_objects.front().get())->start_fire();
	}
}

void Ship::handle_mouse_button_up(uint8_t but)
{
	switch (but) {
	case mouse_button_id::m1:
		static_cast<ShipCannon*>(child_objects.front().get())->stop_fire();
	}
}

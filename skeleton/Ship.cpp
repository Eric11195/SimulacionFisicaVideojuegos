#include "Ship.hpp"
#include "ShipCannon.hpp"
#include <iostream>
#include <cmath>
#include "utils.hpp"
#include "ScreenSizeConstants.hpp"

Ship::Ship()
	:GameObject()
{
	addChild(new ShipCannon());
}

constexpr int max_speed = 3;
void Ship::step(double dt)
{
	GetCamera()->setTransform(global_transform);
	//if(current_angular_velocity)
	if(PxAbs(1.0f - current_angular_velocity.rotation_axis.magnitude()) < 1e-3f)
		rotate(PxQuat(dt*current_angular_velocity.angle, current_angular_velocity.rotation_axis));

	float virar_radians_vel = 1 * dt * (virar_buttons[0] - virar_buttons[1]);
	rotate(PxQuat(virar_radians_vel, PxVec3(0, 0, 1)));
	
	if (current_state != constante) {
		float desired_speed = min(1, max(0, int(current_state)));
		speed += dt * (desired_speed - speed);
		set_vel({ 0,0,max_speed * speed });
	}

	GameObject::step(dt);

}

void Ship::handle_keyboard_button_down(unsigned char c)
{
	switch (c) {
	case ' ':
		static_cast<ShipCannon*>(child_objects.front().get())->start_fire();
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
			static_cast<ShipCannon*>(child_objects.front().get())->stop_fire();
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
//constexpr float max_rot;
void Ship::handle_mouse_pos(float x, float y)
{
	float x_rot, y_rot;
	float px_out_x = 2 *(x - 0.5);
	if (abs(px_out_x) < dead_zone) px_out_x = 0;
	float px_out_y = -2*(y - 0.5);
	if (abs(px_out_y) < dead_zone) px_out_y = 0;

	std::cout << "x: "<<x<<'>'<<px_out_x << "y: "<<y<<'>'<< px_out_y << '\n';

	//lerp from -1 to 1, 
	x_rot = utils::lerp(0.0f, 0.5f, px_out_x);
	y_rot = utils::lerp(0.0f, 0.5f, px_out_y);
	//std::lerp
	PxVec3 normalized_rot_direction = PxVec3(-y_rot,-x_rot, 0);
	float magnitude = normalized_rot_direction.normalize();
	current_angular_velocity = { 3.14f * min(magnitude,1) , normalized_rot_direction };
}

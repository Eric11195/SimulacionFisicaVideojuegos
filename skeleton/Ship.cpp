#include "Ship.hpp"
#include "ShipCannon.hpp"
#include <iostream>
#include <cmath>
#include "utils.hpp"
#include "ScreenSizeConstants.hpp"

Ship::Ship()
	:CompositeGameObject()
{
	addChild(new ShipCannon());
}


void Ship::step(double dt)
{
	CompositeGameObject::step(dt);
	GetCamera()->setTransform(global_transform);
	//if(current_angular_velocity)
	if(PxAbs(1.0f - current_angular_velocity.rotation_axis.magnitude()) < 1e-3f)
		rotate(PxQuat(dt*current_angular_velocity.angle, current_angular_velocity.rotation_axis));

	float virar_radians_vel = 1 * dt * (virar_buttons[0] - virar_buttons[1]);
	std::cout << "rotate_dir: " << virar_radians_vel  << '\n';
	rotate(PxQuat(virar_radians_vel, PxVec3(0, 0, 1)));
	std::cout << "quat: " << local_transform.q.x << ', '<<local_transform.q.y <<', '<<local_transform.q.z<<', '<<local_transform.q.w << '\n';
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
	}

}


constexpr float max_pixels_out_x = 1/ (screen_size_constants::WINDOW_LENGTH_HALF * 0.8);
constexpr float max_pixels_out_y = 1 / (screen_size_constants::WINDOW_HEIGHT_HALF * 0.8);
constexpr int dead_zone = 5;
//constexpr float max_rot;
void Ship::handle_mouse_pos(int x, int y)
{
	float x_rot, y_rot;
	int px_out_x = x - screen_size_constants::WINDOW_LENGTH_HALF;
	if (abs(px_out_x) < dead_zone) px_out_x = 0;
	int px_out_y = -(y - screen_size_constants::WINDOW_HEIGHT_HALF);
	if (abs(px_out_y) < dead_zone) px_out_y = 0;

	//lerp from -1 to 1, 
	x_rot = utils::lerp(0.0f, 0.5f, max(-1,min(1,px_out_x * max_pixels_out_x)));
	y_rot = utils::lerp(0.0f, 0.5f, max(-1, min(1, px_out_y * max_pixels_out_y)));
	//std::lerp
	PxVec3 normalized_rot_direction = PxVec3(-y_rot,-x_rot, 0);
	float magnitude = normalized_rot_direction.normalize();
	//std::cout << magnitude << " <-- " << normalized_rot_direction.magnitude() << '\n';
	current_angular_velocity = { 3.14f * min(magnitude,1) , normalized_rot_direction };
}

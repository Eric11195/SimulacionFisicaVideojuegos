#include "buttons.hpp"
#include "mouse_pos.hpp"

button::button(physx::PxScene* s, std::function<void()> func, const std::string& file_name, physx::PxVec2 start_pos, physx::PxVec2 witdh_height)
	:hud_elem(s, file_name, start_pos, witdh_height), my_click_action(func)
{
}

void button::handle_mouse_button_down(uint8_t mb_id)
{
	if (inside_this_button_range()) {
		click_action();
	}
}

void button::click_action()
{
	my_click_action();
}

bool button::inside_this_button_range()
{
	return texCoords[0].x < mouse_pos_x && mouse_pos_x < texCoords[2].x &&
		texCoords[0].y < mouse_pos_y && mouse_pos_y < texCoords[2].y;
}

#include "InputProcessor.hpp"

void InputProcessor::handle_mouse_pos(int x, int y)
{
}

void InputProcessor::handle_mouse_button_up(uint8_t mb_id)
{
}
void InputProcessor::handle_mouse_button_down(uint8_t mb_id)
{
}

void InputProcessor::handle_keyboard_button_down(unsigned char key)
{
}

void InputProcessor::handle_keyboard_button_up(unsigned char key)
{
}

void InputProcessor::handle_input(event_type ev_t, extra_info ei)
{
	switch (ev_t) {
	case event_type::mouse_pos:
		handle_mouse_pos(ei.my_mouse_pos.x, ei.my_mouse_pos.y);
		break;
	case (event_type::mouse_button | event_type::key_down):
		handle_mouse_button_down(ei.my_mouse_button_id);
		break;
	case (event_type::mouse_button | event_type::key_up):
		handle_mouse_button_up(ei.my_mouse_button_id);
		break;
	case (event_type::key_down | event_type::keyboard_button):
		handle_keyboard_button_down(ei.my_keyboard_key);
		break;
	case (event_type::key_up | event_type::keyboard_button):
		handle_keyboard_button_up(ei.my_keyboard_key);
		break;
	}
}

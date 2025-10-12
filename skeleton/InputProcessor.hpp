#pragma once
#include <cstdint>
#include "core.hpp"

class InputProcessor {
protected:
	enum event_type : uint8_t {
		mouse_pos = 1,
		mouse_button = 1 << 1,
		keyboard_button = 1 << 2,
		key_up = 1 << 3,
		key_down = 1 << 4
	};
	enum mouse_button_id : uint8_t {
		m1 = 0,
		m3 = 1,
		m2 = 2
	};
	struct mouse_pos_data {
		int x, y;
	};
	union extra_info {
		mouse_pos_data my_mouse_pos;
		mouse_button_id my_mouse_button_id;
		unsigned char my_keyboard_key;
	};
public:
	void handle_input(event_type ev_t, extra_info ei);
	virtual void handle_mouse_pos(int x, int y);
	virtual void handle_mouse_button_up(uint8_t mb_id);
	virtual void handle_mouse_button_down(uint8_t mb_id);
	virtual void handle_keyboard_button_down(unsigned char key);
	virtual void handle_keyboard_button_up(unsigned char key);
};
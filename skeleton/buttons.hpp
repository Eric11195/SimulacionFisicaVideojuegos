#pragma once

#include <functional>
#include "Render/hud_elem.hpp"

class button : public hud_elem {
public:
	button(physx::PxScene* s, std::function<void()> func, const std::string& file_name, physx::PxVec2 start_pos = {0,0}, physx::PxVec2 witdh_height = {1, 1});
	void handle_mouse_button_down(uint8_t mb_id) override;
protected:
	std::function<void()> my_click_action;
	virtual void click_action();
	bool inside_this_button_range();
};

#pragma once
#include "CompositeGameObject.hpp"

class Ship : public CompositeGameObject {
public:
	Ship();
	//virtual void process_input(unsigned char c) override;
	virtual void step(double dt) override;
	virtual void handle_keyboard_button_down(unsigned char c) override;
	virtual void handle_keyboard_button_up(unsigned char c) override;
};
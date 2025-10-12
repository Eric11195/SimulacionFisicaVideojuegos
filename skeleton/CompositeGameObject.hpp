#pragma once
#include "GameObject.hpp"
#include <list>
#include <memory>

//Contains multiple GameObjects inside, and it has the tools for calling step and cleanup of its childs
class CompositeGameObject : public GameObject {
public:
	CompositeGameObject(const CompositeGameObject&) = delete;
	CompositeGameObject& operator =(const CompositeGameObject&) = delete;
	CompositeGameObject(config& c = config(), std::initializer_list<GameObject*> = {});
	~CompositeGameObject();
	virtual void addChild(GameObject* go);
	virtual void step(double dt) override;
	//void process_input(unsigned char key) override;
	virtual void handle_mouse_pos(int x, int y) override;
	virtual void handle_mouse_button_up(uint8_t mb_id) override;
	virtual void handle_mouse_button_down(uint8_t mb_id) override;
	virtual void handle_keyboard_button_down(unsigned char key) override;
	virtual void handle_keyboard_button_up(unsigned char key) override;
protected:
	std::list<std::unique_ptr<GameObject>> child_objects;
};

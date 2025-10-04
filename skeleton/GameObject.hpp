#pragma once
#include <list>

struct GameObject {
	GameObject();
	virtual void step(double dt) {}
	virtual void init() {}
	virtual void cleanup();
	virtual void process_input(unsigned char key) {}
	//-------------------------------------------------------
	static void step_all(double dt);
	static void release_all();
	static void process_input_all(unsigned char key);
	//-------------------------------------------------------
protected:
	inline static std::list<GameObject*> GameObject_list{};
	std::list<GameObject*>::iterator my_it;
};
#pragma once
#include "GameObject.hpp"
#include <list>

//Contains multiple GameObjects inside, and it has the tools for calling step and cleanup of its childs
class CompositeGameObject : public GameObject {
public:
	CompositeGameObject(Transform t = Transform(0,0,0));
	void addChild(GameObject* go);
	void step(double dt) override;
	void cleanup() override;
	void process_input(unsigned char key) override;
protected:
	std::list<GameObject*> child_objects;
};
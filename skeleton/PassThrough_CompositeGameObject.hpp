#pragma once
#include "GameObject.hpp"

class PassThrough_CompositeGameObject : public GameObject{
public:
	PassThrough_CompositeGameObject(config& c, Transform const& tr, std::initializer_list<GameObject*> go_s);
	virtual void step(double dt) override;
	virtual std::list<std::unique_ptr<GameObject>>::iterator addChild(GameObject* go) override;
	//virtual void update_position(Transform const& tr) override;
private:
	const Transform& parent_transform;
};
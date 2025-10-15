#include "GameObject.hpp"

GameObject::GameObject(config& c, std::initializer_list<GameObject*> go_s)
	: local_transform(Transform(c.pos.turn(),c.initial_rotation)),
	global_transform(Transform(c.pos.turn(),c.initial_rotation)),
	vel(c.initial_speed_dir.normalize()*c.initial_speed_magnitude),
	accel(c.initial_accel_dir.normalize()*c.initial_accel_magnitude),
	damping_mult(c.damping_mult)
{
	for (auto go : go_s) {
		addChild(go);
	}
}

GameObject::~GameObject()
{
	child_objects.clear();
}

std::list<std::unique_ptr<GameObject>>::iterator GameObject::addChild(GameObject* go)
{
	auto aux_it = child_objects.insert(child_objects.end(), std::unique_ptr<GameObject>(go));
	(*aux_it)->link_to_parent(global_transform);
	return aux_it;
}

Vector3 GameObject::get_pos()
{
	return global_transform.p;
}

void GameObject::step(double dt)
{
	integrate(dt);

	for (auto& child : child_objects) {
		child->step(dt);
		child->update_position(global_transform);
	}
}
void GameObject::translate(physx::PxVec3 t)
{
	local_transform.p = local_transform.q.rotate(t) + local_transform.p;//, q* src.q
	//local_transform.transform(t);
	//translate_to(local_transform.p + t);
}

void GameObject::translate_to(physx::PxVec3 t)
{
	local_transform.p = local_transform.q.rotate(t);
	//local_transform.p = t;
}

void GameObject::rotate(physx::PxQuat q)
{
	local_transform.q *= q;
}

void GameObject::set_accel(My_Vector3 new_accel)
{
	accel = new_accel;
}

void GameObject::add_accel(My_Vector3 add_accel)
{
	accel += add_accel;
}

void GameObject::set_vel(My_Vector3 new_vel)
{
	vel = new_vel;
}

#ifdef DAMPING
void GameObject::set_dumping(float f)
{
	damping_mult = f;
}
#endif
void GameObject::integrate(double dt)
{

#if defined EULER_SEMI_EXPLICIT_INTEGRATION
	vel += accel * dt;
	//tr.p += dt * vel.turn();
	translate(dt * vel.turn());
#elif defined EULER_INTEGRATION
	tr.p += dt * vel.turn();
	vel += accel * dt;
#endif

#ifdef DAMPING
	vel *= pow(damping_mult, dt);
#endif

}
void GameObject::link_to_parent(Transform const& parent_tr)
{
	update_position(parent_tr);
}
void GameObject::update_position(Transform const& parent_tr)
{
	//global_transform = parent_tr.transform(local_transform);
	global_transform = parent_tr.transform(local_transform);
}


void GameObject::handle_mouse_pos(float x, float y)
{
	for (auto& child : child_objects)
		child->handle_mouse_pos(x, y);
}

void GameObject::handle_mouse_button_up(uint8_t mb_id)
{
	for (auto& child : child_objects)
		child->handle_mouse_button_up(mb_id);
}

void GameObject::handle_mouse_button_down(uint8_t mb_id)
{
	for (auto& child : child_objects)
		child->handle_mouse_button_down(mb_id);
}

void GameObject::handle_keyboard_button_down(unsigned char key)
{
	for (auto& child : child_objects)
		child->handle_keyboard_button_down(key);
}

void GameObject::handle_keyboard_button_up(unsigned char key)
{
	for (auto& child : child_objects)
		child->handle_keyboard_button_up(key);
}
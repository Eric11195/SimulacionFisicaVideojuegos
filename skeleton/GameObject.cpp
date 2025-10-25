#include "GameObject.hpp"
#include "ForceGenerator.hpp"
#include <iostream>

GameObject::GameObject(config& c, std::initializer_list<GameObject*> go_s)
	: local_transform(Transform(c.pos,c.initial_rotation)),
	global_transform(Transform(c.pos,c.initial_rotation)),
	vel(c.initial_speed_dir.getNormalized()*c.initial_speed_magnitude),
	damping_mult(c.damping_mult), m(c.inverse_mass)
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

Vector3 GameObject::get_vel() const
{
	return vel;
}

void GameObject::step(double dt)
{
	integrate(dt);

	for (auto& child : child_objects) {
		child->step(dt);
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

void GameObject::set_velocity(physx::PxVec3 v)
{
	vel = v;
}

void GameObject::set_vel(physx::PxVec3 new_vel)
{
	vel = new_vel;
}

#ifdef DAMPING
void GameObject::set_dumping(float f)
{
	damping_mult = f;
}
#endif
void GameObject::calculate_global_to_local_rot()
{
	global_to_local_rot = global_transform.q.getConjugate();
}
void GameObject::integrate(double dt)
{

#if defined EULER_SEMI_EXPLICIT_INTEGRATION
	//In theory this does not exist
	physx::PxVec3 accel = {0,0,0};
	for (auto& force : forces_applied_to_this_obj) {
		//Get matrix transformation only on rotation, to pass from global to local
		auto new_accel = global_to_local_rot.rotate(force->apply_force(*this));
		accel += new_accel;
	}
	//F = m * a, así que si solo le añado todas las fuerzas a accel. Antes de poder añadirselo a la velocidad tengo que dividirlo por la masa (o multiplicarlo por la masa inversa)
	accel *= m.inv_mass;
	//std::cout << vel.x << " " << vel.y << " " << vel.z << '\n';
	vel += accel * dt;
	//tr.p += dt * vel.turn();
	translate(dt * vel);
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
	global_transform = parent_tr.transform(local_transform);

	calculate_global_to_local_rot();

	for (auto& child : child_objects)
		child->update_position(global_transform);
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

void GameObject::add_force_to_myself(std::shared_ptr<ForceGenerator> f)
{
	//This ptr is not created correctly
	forces_applied_to_this_obj.insert(forces_applied_to_this_obj.end(), f);
}

void GameObject::add_force_to_myself(std::string name)
{
	auto it = force_generators_map.find(name);
	if (it == force_generators_map.end())
		throw "No generator exist with that name";
	//This ptr is not created correctly
	add_force_to_myself((*it).second);
}

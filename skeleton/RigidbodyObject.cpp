#include "RigidbodyObject.hpp"
#include <cassert>
#include "ForceGenerator.hpp"
#include <iostream>

Rigidbody_Object::Rigidbody_Object(PxScene* s, PxShape* sh, config& cfg)
	: SceneObject(s, sh, set_rb(physics_ref->createRigidDynamic(PxTransform(PxIDENTITY::PxIdentity))), cfg), my_scene(s)
{
	rb->setLinearVelocity(cfg.go_config.initial_speed_dir*cfg.go_config.initial_speed_magnitude);
	rb->setGlobalPose(global_transform);
	rb->setAngularVelocity({ 0,0,0 });
	rb->attachShape(*sh);
	PxRigidBodyExt::updateMassAndInertia(*rb, cfg.go_config.mass.mass);
	s->addActor(*rb);
}
Rigidbody_Object::~Rigidbody_Object()
{
	my_scene->removeActor(*rb);
	if ((render_item) != nullptr)
		render_item->release();
}
Rigidbody_Object::Rigidbody_Object(PxScene* s, PxShape* sh, config& cfg, NO_REPRESENTATION)
	: SceneObject(s, set_rb(physics_ref->createRigidDynamic(PxTransform(PxIDENTITY::PxIdentity))), cfg)
{
	rb->setLinearVelocity(cfg.go_config.initial_speed_dir * cfg.go_config.initial_speed_magnitude);
	rb->setGlobalPose(global_transform);
	rb->setAngularVelocity({ 0,0,0 });
	rb->attachShape(*sh);
	PxRigidBodyExt::updateMassAndInertia(*rb, cfg.go_config.mass.mass);
	s->addActor(*rb);
}

void Rigidbody_Object::set_velocity(physx::PxVec3 v)
{
	rb->setLinearVelocity(v);
}

PxRigidDynamic* Rigidbody_Object::set_rb(PxRigidDynamic* new_rb)
{
	assert(new_rb);

	rb = new_rb;
	return rb;
}

void Rigidbody_Object::set_my_transform_to_actor()
{
	setTransform(rb->getGlobalPose());
}
PxActor* Rigidbody_Object::getActor()
{
	return rb;
}


void Rigidbody_Object::set_pos(physx::PxVec3 v)
{
	SceneObject::set_pos(v);
	rb->setGlobalPose (global_transform);
}

Vector3 Rigidbody_Object::get_pos()
{
	return global_transform.p;// = rb->getGlobalPose().p;
}

Vector3 Rigidbody_Object::get_vel()
{
	return vel = rb->getLinearVelocity();
}

void Rigidbody_Object::step(double dt)
{
	//auto tr = rb->getGlobalPose();
	setTransform(rb->getGlobalPose());
	SceneObject::step(dt);
}

void Rigidbody_Object::add_torque(physx::PxVec3 add_t)
{
	rb->addTorque(add_t);
}

void Rigidbody_Object::translate(physx::PxVec3 add_p)
{
	//SceneObject::translate(add_p);
	rb->setGlobalPose(global_transform);
}

void Rigidbody_Object::translate_to(physx::PxVec3 p)
{
	set_pos(p);
}

void Rigidbody_Object::set_velocity_magnitude(float m)
{
	get_vel();
	vel /= vel.magnitude();
	vel *= m;
	rb->setLinearVelocity(vel);
}

Transform Rigidbody_Object::get_global_tr()
{
	return rb->getGlobalPose();
}

void Rigidbody_Object::add_speed(physx::PxVec3 v)
{
	rb->addForce(v);
}

void Rigidbody_Object::set_actor_flags(physx::PxActorFlag::Enum f, bool val)
{
	rb->setActorFlag(f, val);
}

void Rigidbody_Object::integrate(double dt)
{
#if defined(EULER_SEMI_IMPLICIT_INTEGRATION) || (!defined(EULER_SEMI_IMPLICIT_INTEGRATION) && !defined(EULER_INTEGRATION))
	//In theory this does not exist
	physx::PxVec3 force_in_newtons = { 0,0,0 };
	for (auto& force : forces_applied_to_this_obj) {
		//Get matrix transformation only on rotation, to pass from global to local
		force_in_newtons += force->apply_force(*this);
	}

	//std::cout << force_in_newtons.x << ' ' << force_in_newtons.y << ' ' << force_in_newtons.z << '\n';
	//F = m * a <=> F/m = a así que si solo le añado todas las fuerzas a accel. Antes de poder añadirselo a la velocidad tengo que dividirlo por la masa (o multiplicarlo por la masa inversa)
	//std::cout << vel.x << " " << vel.y << " " << vel.z << '\n';
	add_speed(dt*force_in_newtons);
	//translate(dt * vel); //COMMENTED BECAUSE I GUESS PHYSICS DOES THIS BY ITSELF
#elif defined EULER_INTEGRATION
	translate(dt * vel);
	physx::PxVec3 accel = { 0,0,0 };
	for (auto& force : forces_applied_to_this_obj) {
		//Get matrix transformation only on rotation, to pass from global to local
		auto new_accel = force->apply_force(*this);
		//new_accel = global_to_local_rot.rotate(new_accel);
		accel += new_accel;
	}
	//F = m * a, así que si solo le añado todas las fuerzas a accel. Antes de poder añadirselo a la velocidad tengo que dividirlo por la masa (o multiplicarlo por la masa inversa)
	accel *= mass.inv_mass;
	vel += accel * dt;
#endif

#ifdef DAMPING
	vel *= pow(damping_mult, dt);
#endif

}


Rigid_SphereObject::Rigid_SphereObject(PxScene* s, SphereObject::config& c)
	: Rigidbody_Object(s, CreateShape(PxSphereGeometry(c.radius)),c.so_config)
{
}

Rigid_SphereObject::Rigid_SphereObject(PxScene* s, SphereObject::config& c, NO_REPRESENTATION nr)
	: Rigidbody_Object(s, CreateShape(PxSphereGeometry(c.radius)), c.so_config, nr)
{
}

Rigid_CubeObject::Rigid_CubeObject(physx::PxScene* s, CubeObject::config& c)
	: Rigidbody_Object(s, CreateShape(PxBoxGeometry(c.half_extents)), c.so_config)
{
}

Rigid_CubeObject::Rigid_CubeObject(physx::PxScene* s, CubeObject::config& c, NO_REPRESENTATION nr)
	: Rigidbody_Object(s, CreateShape(PxBoxGeometry(c.half_extents)), c.so_config, nr)
{
}

//vvvvvvvvv STATIC vvvvvvvvvvv --------------------------------------------------------------------------------------------------------------------------------------------------------------------

StaticRigidbody_Object::StaticRigidbody_Object(PxScene* s, PxShape* sh, config& c)
	: SceneObject(s, sh, set_rb(physics_ref->createRigidStatic(PxTransform(PxIDENTITY::PxIdentity))), c.so_config)
{
	rb->setGlobalPose(global_transform);
	//rb = physics_ref->createRigidDynamic(global_transform);
	//rb->setAngularVelocity({ 0,0,0 });
	rb->attachShape(*sh);
	//PxRigidBodyExt::updateMassAndInertia(*rb, cfg.so_config.go_config.mass.mass);
	s->addActor(*rb);
	//rb->setMassSpaceInertiaTensor
}

void StaticRigidbody_Object::set_velocity(physx::PxVec3)
{
}

PxRigidStatic* StaticRigidbody_Object::set_rb(PxRigidStatic* new_rb)
{
	assert(new_rb);

	rb = new_rb;
	return rb;
}

StaticRigid_SphereObject::StaticRigid_SphereObject(PxScene* s, config& c)
	: StaticRigidbody_Object(s, CreateShape(PxSphereGeometry(c.radius)), c.rb_config)
{
}

StaticRigid_CubeObject::StaticRigid_CubeObject(physx::PxScene* s, config& c)
	: StaticRigidbody_Object(s, CreateShape(PxBoxGeometry(c.half_extents)), c.rb_config)
{
}
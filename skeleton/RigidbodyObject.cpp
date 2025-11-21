#include "RigidbodyObject.hpp"
#include <cassert>
#include "RenderGO.hpp"

Rigidbody_Object::Rigidbody_Object(PxScene* s, PxShape* sh, config& cfg)
	: GameObject(s,cfg.go_config)//SceneObject(s, sh, set_rb(physics_ref->createRigidDynamic(PxTransform(PxIDENTITY::PxIdentity))), cfg.so_config)
{
	//PxRigidDynamic* new_solid;
	rb = physics_ref->createRigidDynamic(PxTransform({ -10,200,-10 }));
	rb->setLinearVelocity({ 0,5,0 });
	rb->setAngularVelocity({ 0,0,0 });
	PxShape* shape_ad = CreateShape(PxBoxGeometry(5, 5, 5));
	rb->attachShape(*shape_ad);
	PxRigidBodyExt::updateMassAndInertia(*rb, 0.15);
	s->addActor(*rb);

	//RenderItem* dynamic_item;
	r_it = new RenderItem(shape_ad, rb, { 0.8,0.8,0.8,1 });
	/*
	rb->setGlobalPose(global_transform);
	//rb = physics_ref->createRigidDynamic(global_transform);
	rb->setAngularVelocity({ 0,90,0 });
	rb->attachShape(*sh);
	PxRigidBodyExt::updateMassAndInertia(*rb, cfg.so_config.go_config.mass.mass);
	s->addActor(*rb);
	*/
	//rb->setMassSpaceInertiaTensor
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


void Rigidbody_Object::set_pos(physx::PxVec3 v)
{
	rb->setGlobalPose ({ v,global_transform.q });
}

void Rigidbody_Object::render3D()
{
	render_go::render3D(r_it);
}

void Rigidbody_Object::step(double dt)
{
}


Rigid_SphereObject::Rigid_SphereObject(PxScene* s, config& c)
	: Rigidbody_Object(s, CreateShape(PxSphereGeometry(c.radius)),c.rb_config)
{
}

Rigid_CubeObject::Rigid_CubeObject(physx::PxScene* s, config& c)
	: Rigidbody_Object(s, CreateShape(PxBoxGeometry(c.half_extents)), c.rb_config)
{
}

//vvvvvvvvv STATIC vvvvvvvvvvv --------------------------------------------------------------------------------------------------------------------------------------------------------------------

StaticRigidbody_Object::StaticRigidbody_Object(PxScene* s, PxShape* sh, config& c)
	: GameObject(s, c.go_config) //SceneObject(s, sh, set_rb(physics_ref->createRigidStatic(PxTransform(PxIDENTITY::PxIdentity))), c.go_config)
{
	rb = physics_ref->createRigidStatic(PxTransform({ 0,0,0 }));
	PxShape* shape = CreateShape(PxBoxGeometry(100, 0.1, 100));
	rb->attachShape(*shape);
	s->addActor(*rb);

	//RenderItem* item;
	r_it = new RenderItem(shape, rb, { 0.8, 0.8, 0.8, 1 });

	/*
	//PxRigidDynamic* new_solid;
	rb = physics_ref->createRigidStatic(PxTransform({ -10,200,-10 }));
	//rb->setLinearVelocity({ 0,5,0 });
	//rb->setAngularVelocity({ 0,0,0 });
	PxShape* shape_ad = CreateShape(PxBoxGeometry(5, 5, 5));
	rb->attachShape(*shape_ad);
	//PxRigidBodyExt::updateMassAndInertia(*rb, 0.15);
	s->addActor(*rb);

	//RenderItem* dynamic_item;
	r_it = new RenderItem(shape_ad, rb, { 0.8,0.8,0.8,1 });
	*/
	/*
	rb->setGlobalPose(global_transform);
	//rb = physics_ref->createRigidDynamic(global_transform);
	//rb->setAngularVelocity({ 0,0,0 });
	rb->attachShape(*sh);
	//PxRigidBodyExt::updateMassAndInertia(*rb, cfg.so_config.go_config.mass.mass);
	s->addActor(*rb);
	//rb->setMassSpaceInertiaTensor
	*/
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

void StaticRigidbody_Object::render3D()
{
	render_go::render3D(r_it);
}

StaticRigid_SphereObject::StaticRigid_SphereObject(PxScene* s, config& c)
	: StaticRigidbody_Object(s, CreateShape(PxSphereGeometry(c.radius)), c.rb_config)
{
}

StaticRigid_CubeObject::StaticRigid_CubeObject(physx::PxScene* s, config& c)
	: StaticRigidbody_Object(s, CreateShape(PxBoxGeometry(c.half_extents)), c.rb_config)
{
}
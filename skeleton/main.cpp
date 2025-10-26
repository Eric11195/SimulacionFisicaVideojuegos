#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"


#include <iostream>
#include "CoordinateAxis.hpp"
#include "Particle.hpp"
#include "Projectile.hpp"
#include "CameraProjectileShooter.hpp"
#include "GameObject.hpp"
#include "GlobalCoords_CompositeGameObject.hpp"
#include "ParticleGenerator.hpp"
#include "ParticleGeneratorsDescriptors.hpp"
#include "ParticleDescriptor.hpp"
#include "ParticleSystem.hpp"
#include "Ship.hpp"
#include "ForceGenerator.hpp"
#include "EnemyShip.hpp"
#include "BlackHole.hpp"

std::string display_text = "This is a test";
CoordinateAxis* co=nullptr;

using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;


PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene      = NULL;
ContactReportCallback gContactReportCallback;

GlobalCoords_CompositeGameObject* scene_game_object = nullptr;

// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(),true,gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);

	//INSTANTIATE SCENE NODE
	scene_game_object = new GlobalCoords_CompositeGameObject();
	//CREATE ALL FORCE GENERATORS:
	scene_game_object->addChild(new Gravity_ForceGenerator(physx::PxVec3( 0,-1,0 )));
		//(new Directional_ForceGenerator("gravity", { 0,-1,0 }, 0.98f));

	//------------------------------

	scene_game_object->addChild(new CoordinateAxis());
	
	Projectile::projectile_config c = 
		{ ParticleDescriptor::regular_ball,
		30,//SPEED REAL
		0.5f, //MASS REAL
		};

	scene_game_object->addChild(new CameraProjectileShooter(c));


	//scene_game_object->addChild(new BlackHole({ 5,5,5 }, 1));
	//scene_game_object->addChild(new ForceAffectedParticleGenerator(testing_blackhole_particles, "black_hole"));

	auto player = new Ship();
	scene_game_object->addChild(player);
	for (int i = 0; i < 3; ++i) {
		scene_game_object->addChild(new EnemyShip(player));
	}
	//scene_game_object->addChild(new ParticleSystem({ new ParticleGenerator(ParticleGeneratorsDescriptors::ball_thrower) }));
	//new Projectile(c);
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);
	scene_game_object->step(t);
	scene_game_object->update_position(PhysicLib::NEUTRAL_TRANSFORM);
	gScene->simulate(t);
	gScene->fetchResults(true);
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();	
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
	
	gFoundation->release();
}

void keyPress(unsigned char key)
{
	scene_game_object->handle_keyboard_button_down(key);
}

void keyRelease(unsigned char key)
{
	scene_game_object->handle_keyboard_button_up(key);
}
void mouseReleased(uint8_t button) {
	scene_game_object->handle_mouse_button_up(button);
}
void mousePressed(uint8_t button) {
	scene_game_object->handle_mouse_button_down(button);
}
void mousePosUpdated(float x, float y) {
	scene_game_object->handle_mouse_pos(x,y);
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}


int main(int, const char*const*)
{
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for(PxU32 i=0; i<frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}
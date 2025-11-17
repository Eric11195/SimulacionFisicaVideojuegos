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

#include "GameObject.hpp"
#include "ParticleGenerator.hpp"
#include "ParticleGeneratorsDescriptors.hpp"
#include "ParticleDescriptor.hpp"
#include "ParticleSystem.hpp"
#include "Ship.hpp"
#include "ForceGenerator.hpp"
#include "EnemyShip.hpp"
#include "BlackHole.hpp"
#include "PointSpring.hpp"
#include "Render/hud_elem.hpp"
#include "buttons.hpp"

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

GameObject* current_scene = nullptr;
enum scenes {
	gamescene,
	mainmenu,
	max_number
};

scenes starting_scene = mainmenu;
GameObject* scenes_vec[scenes::max_number];


GameObject* get_rendering_obj() {
	return current_scene;
}

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
	scenes_vec[gamescene] = new GameObject();
	//CREATE ALL FORCE GENERATORS:
	scenes_vec[gamescene]->addChild(new Gravity_ForceGenerator("gravity", physx::PxVec3(0, -1, 0)));

	//------------------------------

	scenes_vec[gamescene]->addChild(new CoordinateAxis());

	scenes_vec[gamescene]->addChild(new BlackHole({ 5,5,5 }, 1));
	//scene_game_object->addChild(new ForceAffected_ParticleGenerator(testing_blackhole_particles, {"black_hole", "gravity"}));
	//scene_game_object->addChild(new ForceAffected_ParticleGenerator(testing_blackhole_particles, "black_hole"));

	auto player = new Ship();
	scenes_vec[gamescene]->addChild(player);
	for (int i = 0; i < 10; ++i) {
		scenes_vec[gamescene]->addChild(new EnemyShip(player));
	}

	//Muelles----------------------------------------------
	//const std::string spring_name = "my_first_spring";
	//auto spring_force = new PointSpring(physx::PxVec3(0,0,0), Spring_ForceGenerator::config{ 1, 0.01 }, spring_name);//new PT_OBJ_Spring_ForceGenerator("spring", {1000, 10});
	//scenes_vec[gamescene]->addChild(spring_force);

	//auto p_gen = new ForceAffected_ParticleGenerator(missile_particle_system, {  "spring" });
	//p_gen->translate_to({ 0,0,0 });
	//scenes_vec[gamescene]->addChild(p_gen);
	//-----------------------------------------------------

	scenes_vec[gamescene]->addChild(new hud_elem("casco_nave.png"));

	//MAIN MENU SCENE

	scenes_vec[mainmenu] = new GameObject();

	auto start_but = new button([&] {
		current_scene = scenes_vec[gamescene];
		},
		"start_game_button.png", { 0.33,0.4 }, {0.33,0.20});
	scenes_vec[mainmenu]->addChild(start_but);

	scenes_vec[mainmenu]->addChild(new text_hud_elem("Creado por Izan de Vega", {0.01,0.01}));

	for (int i = 0; i < 20; ++i) {
		scenes_vec[mainmenu]->addChild(new EnemyShip(scenes_vec[mainmenu]));
	}
	GetCamera()->setTransform(scenes_vec[mainmenu]->get_global_tr());


	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	current_scene = scenes_vec[starting_scene];
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);
	current_scene->step(t);
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
	current_scene->handle_keyboard_button_down(key);
}

void keyRelease(unsigned char key)
{
	current_scene->handle_keyboard_button_up(key);
}
void mouseReleased(uint8_t button) {
	current_scene->handle_mouse_button_up(button);
}
void mousePressed(uint8_t button) {
	current_scene->handle_mouse_button_down(button);
}
void mousePosUpdated(float x, float y) {
	current_scene->handle_mouse_pos(x,y);
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
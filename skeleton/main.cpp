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
#include "RigidbodyObject.hpp"

std::string display_text = "This is a test";
CoordinateAxis* co=nullptr;

std::unordered_map<PxActor*, ShipInterface*> ships;
Ship* player;

using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;


PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
//PxScene*				gScene      = NULL;
ContactReportCallback gContactReportCallback;

GameObject* current_scene = nullptr;
physx::PxScene* physx_current_scene = nullptr;
enum scenes {
	gamescene,
	mainmenu,
	max_number
};

scenes starting_scene = mainmenu;
GameObject* scenes_vec[scenes::max_number];
physx::PxScene* physx_scene_vec[scenes::max_number];


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
	GameObject::physics_ref = gPhysics;

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	physx_scene_vec[gamescene] = gPhysics->createScene(sceneDesc);

	//INSTANTIATE SCENE NODE
	scenes_vec[gamescene] = new GameObject(physx_scene_vec[gamescene]);
	//CREATE ALL FORCE GENERATORS:
	scenes_vec[gamescene]->addChild(new Gravity_ForceGenerator(physx_scene_vec[gamescene], "gravity", physx::PxVec3(0, -1, 0)));

	//------------------------------

	scenes_vec[gamescene]->addChild(new CoordinateAxis(physx_scene_vec[gamescene]));

	scenes_vec[gamescene]->addChild(new BlackHole(physx_scene_vec[gamescene], { 5,5,5 }, 1));
	//scene_game_object->addChild(new ForceAffected_ParticleGenerator(testing_blackhole_particles, {"black_hole", "gravity"}));
	//scene_game_object->addChild(new ForceAffected_ParticleGenerator(testing_blackhole_particles, "black_hole"));

	ships = std::unordered_map<PxActor*, ShipInterface*>{};
	player = new Ship(physx_scene_vec[gamescene]);
	ships[player->getActor()] = player;
	player->assign_die_func(
		[&]() {
			current_scene = scenes_vec[mainmenu];
			physx_current_scene = physx_scene_vec[mainmenu];
		}
	);
	scenes_vec[gamescene]->addChild(player);
	for (int i = 0; i < 10; ++i) {
		auto en = new EnemyShip(physx_scene_vec[gamescene], player);
		ships[en->getActor()] = en;
		scenes_vec[gamescene]->addChild(en);
	}

	//TESTING SOLIDO RIGIDO
	/*
	scenes_vec[gamescene]->addChild(new StaticRigid_CubeObject(physx_scene_vec[gamescene], 
		StaticRigid_CubeObject::config{ StaticRigidbody_Object::config(), physx::PxVec3(100,0.01,100)} ));

	auto cube = scenes_vec[gamescene]->addChild(new Rigid_CubeObject(physx_scene_vec[gamescene],
		Rigid_CubeObject::config{ Rigidbody_Object::config{{SceneObject::config{GameObject::config(), Color(1,0,0,1)}}}, physx::PxVec3(1,1,1)}));
	(*cube)->set_pos({ 0,0,11 });
	(*cube)->set_velocity({ 0,30,0 });
	*/

	//---------------------


	//Muelles----------------------------------------------
	//const std::string spring_name = "my_first_spring";
	//auto spring_force = new PointSpring(physx::PxVec3(0,0,0), Spring_ForceGenerator::config{ 1, 0.01 }, spring_name);//new PT_OBJ_Spring_ForceGenerator("spring", {1000, 10});
	//scenes_vec[gamescene]->addChild(spring_force);

	//auto p_gen = new ForceAffected_ParticleGenerator(missile_particle_system, {  "spring" });
	//p_gen->translate_to({ 0,0,0 });
	//scenes_vec[gamescene]->addChild(p_gen);
	//-----------------------------------------------------

	scenes_vec[gamescene]->addChild(new hud_elem(physx_scene_vec[gamescene], "casco_nave.png"));

	//MAIN MENU SCENE
	physx_scene_vec[mainmenu] = gPhysics->createScene(sceneDesc);
	scenes_vec[mainmenu] = new GameObject(physx_scene_vec[mainmenu]);

	auto start_but = new button(physx_scene_vec[gamescene], [&] {
		current_scene = scenes_vec[gamescene];
		physx_current_scene = physx_scene_vec[gamescene];
		},
		"start_game_button.png", { 0.33,0.65 }, {0.33,0.20}
	);
	scenes_vec[mainmenu]->addChild(start_but);
	scenes_vec[mainmenu]->addChild(new hud_elem(physx_scene_vec[gamescene], "logo_juego.png", { 0.25, 0.1 }, {0.5,0.5}));
	scenes_vec[mainmenu]->addChild(new text_hud_elem(physx_scene_vec[gamescene], "Creado por Izan de Vega", {0.01,0.01}));

	for (int i = 0; i < 20; ++i) {
		scenes_vec[mainmenu]->addChild(new EnemyShip(physx_scene_vec[gamescene], scenes_vec[mainmenu]));
	}
	GetCamera()->setTransform(scenes_vec[mainmenu]->get_global_tr());


	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	current_scene = scenes_vec[starting_scene];
	physx_current_scene = physx_scene_vec[starting_scene];
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);
	current_scene->step(t);
	physx_current_scene->simulate(t);
	physx_current_scene->fetchResults(true);
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	physx_current_scene->release();
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

	std::vector<std::unordered_map<PxActor*, ShipInterface*>::iterator> its = { ships.find(actor1), ships.find(actor2) };

	int n_ships = (its[0] != ships.end()) + 2*(its[1] != ships.end());
	ShipInterface* the_ship;// = its[0]->second;
	switch (n_ships) {
	case 2:
		the_ship = its[1]->second;
		the_ship->die();
		break;
	case 1:
		//A ship has bumped into something else
		the_ship = its[0]->second;
		the_ship->die();
		break;
	case 3: {
		//The ship will be the player
		bool player_here = false;
		if (its[0]->second == player || its[1]->second==player) {
			the_ship = player;
			player_here = true;
		}

		if (player_here) {
			//the_ship->die();
		}
		else {
			//Other ships
		}
		//A ship has bumped into another ship
		break;
	}
	default:
		//UNIMPORTANT THINGS HAPPENED
		break;
	}

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
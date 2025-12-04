#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>
#include <string>

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
#include "Render/hud_elem.hpp"
#include "buttons.hpp"
#include "RigidbodyObject.hpp"


constexpr uint8_t STARTING_NUMBER_OF_ELEMENTS = 10;
uint8_t remaining_enemies = STARTING_NUMBER_OF_ELEMENTS;

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
	endgame,
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

	//scenes_vec[gamescene]->addChild(new BlackHole(physx_scene_vec[gamescene], { 5,5,5 }, 1));
	//scene_game_object->addChild(new ForceAffected_ParticleGenerator(testing_blackhole_particles, {"black_hole", "gravity"}));
	//scene_game_object->addChild(new ForceAffected_ParticleGenerator(testing_blackhole_particles, "black_hole"));


	scenes_vec[gamescene]->addChild(new hud_elem(physx_scene_vec[gamescene], "casco_nave.png"));
	auto my_n_enemies_text = new text_hud_elem(physx_scene_vec[gamescene], ("Enemies Remaining: " + std::to_string(STARTING_NUMBER_OF_ELEMENTS)), { 0.8f,0.8f });
	scenes_vec[gamescene]->addChild(my_n_enemies_text);

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
		auto enemy_it = scenes_vec[gamescene]->addChild(en);
		
		en->assign_die_func(
			[&, my_n_enemies_text]() {
				--remaining_enemies;
				//std::cout << std::to_string(remaining_enemies) << '\n';
				my_n_enemies_text->change_text("Enemies Remaining: " + std::to_string(remaining_enemies));
				if (remaining_enemies <= 0) {
					current_scene = scenes_vec[endgame];
					physx_current_scene = physx_scene_vec[endgame];
				}
			}
		);
	}

	//MAIN MENU SCENE
	physx_scene_vec[mainmenu] = gPhysics->createScene(sceneDesc);
	scenes_vec[mainmenu] = new GameObject(physx_scene_vec[mainmenu]);

	auto start_but = new button(physx_scene_vec[mainmenu], [&] {
		current_scene = scenes_vec[gamescene];
		physx_current_scene = physx_scene_vec[gamescene];
		},
		"start_game_button.png", { 0.33,0.65 }, {0.33,0.20}
	);
	scenes_vec[mainmenu]->addChild(start_but);
	scenes_vec[mainmenu]->addChild(new hud_elem(physx_scene_vec[mainmenu], "logo_juego.png", { 0.25, 0.1 }, {0.5,0.5}));
	scenes_vec[mainmenu]->addChild(new text_hud_elem(physx_scene_vec[mainmenu], "Creado por Izan de Vega", {0.01,0.01}));

	for (int i = 0; i < 20; ++i) {
		scenes_vec[mainmenu]->addChild(new EnemyShip(physx_scene_vec[mainmenu], scenes_vec[mainmenu]));
	}
	GetCamera()->setTransform(scenes_vec[mainmenu]->get_global_tr());

	//------------------------------------------------------------------------------------------------------------------------
	physx_scene_vec[endgame] = gPhysics->createScene(sceneDesc);
	scenes_vec[endgame] = new GameObject(physx_scene_vec[endgame]);
	scenes_vec[endgame]->addChild(new button(physx_scene_vec[endgame], []() {
		exit(0);
		}, "exit_button.png", { 0.33,0.65 }, { 0.33,0.20 })
	);
	scenes_vec[endgame]->addChild(new text_hud_elem(physx_scene_vec[endgame], "Felicidades! Has eliminado a todos los enemigos", {0.1,0.8}));

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
	for (auto scene : scenes_vec) {
		delete scene;
	}
	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	for (auto physx_scene : physx_scene_vec) {
		physx_scene->release();
	}
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

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2, physx::PxVec3 contact_point, physx::PxVec3 normal)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);

	std::vector<std::unordered_map<PxActor*, ShipInterface*>::iterator> its = { ships.find(actor1), ships.find(actor2) };

	int n_ships = (its[0] != ships.end()) + 2*(its[1] != ships.end());
	ShipInterface* the_ship;// = its[0]->second;
	switch (n_ships) {
	case 2:
		its[1]->second->set_collision_point(contact_point,normal);
		break;
	case 1:
		its[0]->second->set_collision_point(contact_point,normal);
		break;
	case 3: {
		//The ship will be the player
		//if()
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
#pragma once
#include "Particle.hpp"
#include "PhysicLib.hpp"

Particle::config& regular_ball();

//Particle from which the explosions originates
Particle::config& bomb_init_particle();

//Created by the bomb when it explodes
Particle::config& bomb_created_particles();

Particle::config& black_hole_particles();

Particle::config& laser_shot();

Particle::config& missile_particle();

Particle::config& missile_generated_particles();

Particle::config& propulsores_enemy_ship_particles();
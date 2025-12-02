#include "ShipRegularProjectileCannon.hpp"
#include "ParticleGeneratorsDescriptors.hpp"
#include "Projectile.hpp"
#include "ForceGenerator.hpp"

ShipRegularProjectileCannon::ShipRegularProjectileCannon(physx::PxScene* s)
	: TriggeredParticleGenerator(s, x_wing_shoot_type)
{
	//float real_speed = avrg_speed;
	//float sim_speed = 10;
	//avrg_speed = sim_speed;
	//avrg_mass = Mass(avrg_mass.mass, real_speed,sim_speed));
	//my_mod_gravity = new Directional_ForceGenerator({0,-1,0}, Projectile::get_s_gravity(real_speed,sim_speed));
	//addChild(my_mod_gravity);
	//force_ptr.push_back(my_mod_gravity);
}

GameObject* ShipRegularProjectileCannon::set_up_particle(Particle::config& p, void* v)
{
	auto inertial_speed = *static_cast<physx::PxVec3*>(v);
	auto c = Particle::config{ p.spho_config,p.time_till_death };
	auto my_init_speed = c.spho_config.so_config.go_config.initial_speed_dir * c.spho_config.so_config.go_config.initial_speed_magnitude;
	my_init_speed = my_init_speed + inertial_speed;
	//c.spho_config.so_config.go_config.pos = c.spho_config.so_config.go_config.pos + my_init_speed;
	c.spho_config.so_config.go_config.initial_speed_magnitude = my_init_speed.normalize();
	c.spho_config.so_config.go_config.initial_speed_dir = my_init_speed;

	//auto proj_config = Particle::config{ p };// , 30000.0f};
	auto particle = new Projectile(my_scene, c, 1000000, const_p_config.initial_speed_magnitude);
	particle->my_team_id = player;
	//particle->add_force_to_myself(my_mod_gravity);
	for (auto& f : force_names) {
		particle->add_force_to_myself(f);
	}
	for (auto f : force_ptr) {
		particle->add_force_to_myself(f);
	}
	return particle;
}

#include "ShipRegularProjectileCannon.hpp"
#include "ParticleGeneratorsDescriptors.hpp"
#include "Projectile.hpp"
#include "ForceGenerator.hpp"

ShipRegularProjectileCannon::ShipRegularProjectileCannon()
	: TriggeredParticleGenerator(x_wing_shoot_type)
{
	//float real_speed = avrg_speed;
	//float sim_speed = 10;
	//avrg_speed = sim_speed;
	//avrg_mass = Mass(avrg_mass.mass, real_speed,sim_speed));
	//my_mod_gravity = new Directional_ForceGenerator({0,-1,0}, Projectile::get_s_gravity(real_speed,sim_speed));
	//addChild(my_mod_gravity);
	//force_ptr.push_back(my_mod_gravity);
}

Particle* ShipRegularProjectileCannon::set_up_particle(Particle::config& p)
{
	auto proj_config = Projectile::projectile_config{ p };// , 30000.0f};
	auto particle = new Projectile(proj_config, 1000000, avrg_speed);
	//particle->add_force_to_myself(my_mod_gravity);
	return particle;
}

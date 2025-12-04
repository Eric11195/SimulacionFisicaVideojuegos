#include "SpringJoinedProjectileLauncher.hpp"
#include "ForceGenerator.hpp"


SpringJoinedProjectileLauncher::SpringJoinedProjectileLauncher(physx::PxScene* s)
	:TriggeredParticleGenerator(s, spring_projectile_generator)
{
}

void SpringJoinedProjectileLauncher::generate_particles(double dt, void* ptr)
{
	float particles_this_frame = particles_per_second_accumulator + particle_generated_per_second * dt;
	int particles_generated_in_current_frame = floor(particles_this_frame);
	particles_per_second_accumulator = particles_this_frame - particles_generated_in_current_frame;
	auto& new_p_config_short = p_config.spho_config.so_config.go_config;
	std::vector<GameObject*> v(2);
	for (int i = 0; i < particles_generated_in_current_frame; ++i) {
		p_config.spho_config.radius = avrg_size + my_particle_lambdas.size();
		if (p_config.spho_config.radius <= 0) continue;
		new_p_config_short.initial_rotation = global_transform.q;
		new_p_config_short.pos = global_transform.p + global_transform.q.rotate(const_p_config.pos + my_particle_lambdas.pos());
		//new_p_config_short.initial_accel_magnitude; //= 30;
		//new_p_config_short.initial_accel_dir = const_p_config.initial_accel_dir;
		new_p_config_short.initial_speed_magnitude = const_p_config.initial_speed_magnitude + my_particle_lambdas.vel();
		new_p_config_short.initial_speed_dir = global_transform.q.rotate(const_p_config.initial_speed_dir + my_particle_lambdas.dir());
		new_p_config_short.mass = const_p_config.mass + my_particle_lambdas.mass();
		p_config.time_till_death = avrg_lifetime + my_particle_lambdas.lifetime();
		p_config.spho_config.so_config.color = avrg_color + my_particle_lambdas.color();
		v[i] = set_up_particle(p_config, ptr);
		addChild(static_cast<GameObject*>(v[i]));
	}
	Spring_ForceGenerator::config c{ const_p_config.mass.mass * elastic_const, spring_idle_long };
	auto new_force = new OBJ_OBJ_Spring_ForceGenerator(my_scene, c, v[0]->get_pos_ptr(), v[1]->get_pos_ptr());
	addChild(new_force);
	v[0]->add_force_to_myself(new_force);
	v[1]->add_force_to_myself(new_force);
}

GameObject* SpringJoinedProjectileLauncher::set_up_particle(Particle::config& p, void* v)
{
	auto inertial_speed = *static_cast<physx::PxVec3*>(v);
	auto c = Particle::config{ p.spho_config,p.time_till_death };
	auto my_init_speed = c.spho_config.so_config.go_config.initial_speed_dir * c.spho_config.so_config.go_config.initial_speed_magnitude;
	my_init_speed = my_init_speed + inertial_speed;
	//c.spho_config.so_config.go_config.pos = c.spho_config.so_config.go_config.pos + my_init_speed;
	c.spho_config.so_config.go_config.initial_speed_magnitude = my_init_speed.normalize();
	c.spho_config.so_config.go_config.initial_speed_dir = my_init_speed;

	return new Projectile(my_scene, c);
}

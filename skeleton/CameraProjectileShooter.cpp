#include "CameraProjectileShooter.hpp"
#include <iostream>

CameraProjectileShooter::CameraProjectileShooter
	(Projectile::projectile_config& pc)
	: pr_spawned(pc), GlobalCoords_CompositeGameObject()
{
}

void CameraProjectileShooter::Shoot()
{
	auto cam = GetCamera();
	auto &short_pr_config = pr_spawned.particle_config.spho_config.so_config.go_config;
	short_pr_config.initial_speed_dir = My_Vector3::unturn(cam->getDir());
	short_pr_config.pos = My_Vector3::unturn(cam->getEye());
	addChild(new Projectile(pr_spawned));
}
/*
void CameraProjectileShooter::process_input(unsigned char key)
{
	switch (key) {
	case' ':
		Shoot();
	}
	CompositeGameObject::process_input(key);
}
*/
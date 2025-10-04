#include "CameraProjectileShooter.hpp"
#include <iostream>

CameraProjectileShooter::CameraProjectileShooter
	(Projectile::projectile_config pc)
	: pr_spawned(pc)
{
}

void CameraProjectileShooter::Shoot()
{
	std::cout << "Shoot \n";
	auto cam = GetCamera();
	pr_spawned.initial_dir = My_Vector3::unturn(cam->getDir());
	pr_spawned.position = My_Vector3::unturn(cam->getEye());
	std::cout << "dir: " << pr_spawned.initial_dir << " pos: "<<pr_spawned.position<<'\n';
	addChild(new Projectile(pr_spawned));
}

void CameraProjectileShooter::process_input(unsigned char key)
{
	switch (key) {
	case' ':
		Shoot();
	}
	CompositeGameObject::process_input(key);
}

#include "CameraProjectileShooter.hpp"
#include <iostream>

CameraProjectileShooter::CameraProjectileShooter
	(Projectile::projectile_config pc)
	: pr_spawned(pc), GlobalCoords_CompositeGameObject(Transform(0,0,0))
{
}

void CameraProjectileShooter::Shoot()
{
	auto cam = GetCamera();
	pr_spawned.particle_config.initial_dir = My_Vector3::unturn(cam->getDir());
	pr_spawned.particle_config.pos = My_Vector3::unturn(cam->getEye());
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

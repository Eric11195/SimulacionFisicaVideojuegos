#pragma once
#include "Projectile.hpp"
#include "RenderUtils.hpp"
#include "GameObject.hpp"

class CameraProjectileShooter : GameObject{
public:
	CameraProjectileShooter(Projectile::projectile_config);
	void Shoot();
	void process_input(unsigned char key) override;
private:
	Projectile::projectile_config pr_spawned;
};
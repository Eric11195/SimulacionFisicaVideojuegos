#pragma once
#include "Projectile.hpp"
#include "RenderUtils.hpp"
#include "GameObject.hpp"

/*
Projectile Shotter (Empty)
	-bullets 0..n
*/

class CameraProjectileShooter : public GameObject{
public:
	CameraProjectileShooter(Projectile::projectile_config&);
	void Shoot();
	//void process_input(unsigned char key) override;
private:
	Projectile::projectile_config pr_spawned;
};
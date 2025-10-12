#pragma once
#include "Projectile.hpp"
#include "RenderUtils.hpp"
#include "GameObject.hpp"
#include "GlobalCoords_CompositeGameObject.hpp"

/*
Projectile Shotter (Empty)
	-bullets 0..n
*/

class CameraProjectileShooter : public GlobalCoords_CompositeGameObject{
public:
	CameraProjectileShooter(Projectile::projectile_config&);
	void Shoot();
	//void process_input(unsigned char key) override;
private:
	Projectile::projectile_config pr_spawned;
};
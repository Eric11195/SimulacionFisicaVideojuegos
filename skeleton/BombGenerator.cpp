#include "BombGenerator.hpp"
#include "ForceGenerator.hpp"


Bomb::Bomb(GameObject::config& c, float expansion_force, float force_multiplier)
	:GameObject(c)
{
	addChild(new Variable_ForceGenerator("ExplosionForce", expansion_force,
		[](float force_mag, GameObject const& self, GameObject const& g) {
			const physx::PxVec3 force_applied = (g.get_global_tr().p - self.get_global_tr().p).getNormalized();
			return force_applied*force_mag;
		},
		[force_multiplier](double time, double dt, float& force_mag) {
			force_mag *= force_multiplier*dt;
		}
	));
}

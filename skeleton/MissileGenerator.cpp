#include "MissileGenerator.hpp"
#include "ParticleGeneratorsDescriptors.hpp"
#include <iostream>

/*
MissileCannon::MissileCannon() {
	GameObject* pg = new Missile();
	pg->translate_to({ 0,-10.0f,20 });
	addChild(pg);
}
*/

MissileGenerator::MissileGenerator()
	:TriggeredParticleGenerator(missile)
{
}

void MissileGenerator::set_up_particle(Particle* p)
{
	addChild(p);
	auto c = new ParticleGenerator(missile_particle_system);
	p->addChild(c);
	//p->local_transform.q = local_transform.q;

	/*
	auto& u = c->global_transform.p;
	auto& q = c->global_transform.q;
	std::cout << u.x << " " << u.y << " " << u.z << " --- " << q.x << " " << q.y << " " << q.z << " " << q.w << '\n';
	auto& a = p->global_transform.p;
	auto& b = p->global_transform.q;
	std::cout << a.x << " " << a.y << " " << a.z << " --- " << b.x << " " << b.y << " " << b.z << " " << b.w << '\n' << '\n';

	auto& e = p->global_transform.p;
	auto& d = p->global_transform.q;
	std::cout << e.x << " " << e.y << " " << e.z << " --- " << d.x << " " << d.y << " " << d.z << " " << d.w << '\n' << '\n';
	*/
}

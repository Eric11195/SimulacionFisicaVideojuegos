#include "RigidbodyObject.hpp"

Rigidbody_Object::Rigidbody_Object(PxScene* s, PxShape* sh, config cfg)
	:SceneObject( s, sh, cfg.so_config)
{

}

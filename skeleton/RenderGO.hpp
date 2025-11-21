#pragma once
//#include "core.hpp"
//#include "PxPhysicsAPI.h"
#include "RenderUtils.hpp"



namespace render_go {
	extern void render3D(RenderItem* r_it);
	extern void render_shape(const physx::PxShape& shape, const physx::PxTransform& transform, const physx::PxVec4& color);
	extern void render_geometry(const physx::PxGeometryHolder& h, bool wireframe = false);
}
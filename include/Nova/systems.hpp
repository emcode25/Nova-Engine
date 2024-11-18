#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP

#include <flecs.h>

namespace Nova
{
	void ObjectRenderSystem(flecs::iter& it);
	void PointLightRenderSystem(flecs::iter& it);
}

#endif
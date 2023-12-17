#ifndef OBJECTS_HPP
#define OBJECTS_HPP

#include <flecs.h>

namespace Nova
{
	flecs::entity createCube(const flecs::world& ecs);
	flecs::entity createCamera(const flecs::world& ecs);
}

#endif

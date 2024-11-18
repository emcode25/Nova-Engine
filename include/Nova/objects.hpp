#ifndef OBJECTS_HPP
#define OBJECTS_HPP

#include <flecs.h>
#include <Nova/components.hpp>

namespace Nova
{
	flecs::entity createCube();
	flecs::entity createCamera();
	flecs::entity createLightCube(const Nova::Component::PointLight& props);
}

#endif

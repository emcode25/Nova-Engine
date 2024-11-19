#ifndef OBJECTS_HPP
#define OBJECTS_HPP

#include <Nova/types.hpp>
#include <Nova/components.hpp>

namespace Nova
{
	Nova::Entity createCube();
	Nova::Entity createCamera();
	Nova::Entity createLightCube(const Nova::Component::PointLight& props);
}

#endif

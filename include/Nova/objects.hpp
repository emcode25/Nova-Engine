#ifndef OBJECTS_HPP
#define OBJECTS_HPP

#include <Nova/types.hpp>
#include <Nova/components.hpp>

namespace Nova
{
	Nova::MeshInfo loadCubeMesh();

	Nova::Entity createCube();
	Nova::Entity createCamera();
	Nova::Entity createLightCube(const Nova::Component::PointLight& props);
	Nova::Entity createDefaultPointLight();
}

#endif

#ifndef LIGHT_MANAGER_HPP
#define LIGHT_MANAGER_HPP

#include <vector>
#include <Nova/components.hpp>
#include <flecs.h>
#include <Nova/types.hpp>

namespace Nova
{
	namespace Lighting
	{
		class LightManager
		{
		public:
			LightManager();

			bool addProgram(Nova::ShaderProgram program);
			void removeProgram(Nova::ShaderProgram program);
			void clearPrograms();

			bool addPointLight(Nova::Entity e);
			bool addPointLight(Nova::Component::PointLight pl);
			void removePointLight(Nova::String name);
			void removePointLight(Nova::Entity e);

			void loadPointLight(Nova::Entity e);
			void loadPointLights();
			void deleteLights();

			Nova::UInt getPointLightCount();

		private:
			Nova::Array<Nova::ShaderProgram> programs;
			Nova::Array<Nova::Entity> pointLights;
		};
	}
}

#endif
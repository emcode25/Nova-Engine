#ifndef LIGHT_MANAGER_HPP
#define LIGHT_MANAGER_HPP

#include <glad/glad.h>
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

			bool addPointLight(Nova::Entity e);
			bool addPointLight(Nova::Component::PointLight pl);
			void removePointLight(Nova::String name);
			void removePointLight(Nova::Entity e);

			Nova::UInt getPointLightCount();

		private:
			void loadPointLights();

			Nova::Array<Nova::ShaderProgram> programs;
			Nova::Array<Nova::Entity> pointLights;
		}
	}
};

#endif
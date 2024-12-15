#include <Nova/light_manager.hpp>

#include <iostream>
#include <glad/glad.h>

Nova::Lighting::LightManager::LightManager()
{
}

bool Nova::Lighting::LightManager::addProgram(Nova::ShaderProgram program)
{
	programs.push_back(program);

	loadPointLights();

	return true;
}

void Nova::Lighting::LightManager::removeProgram(Nova::ShaderProgram program)
{
}

void Nova::Lighting::LightManager::clearPrograms()
{
	programs.clear();
}

bool Nova::Lighting::LightManager::addPointLight(Nova::Entity e)
{
	if (!e.has<Nova::Component::PointLight>())
	{
		std::cerr << "WARNING: Entity does not contain a PointLight component." << std::endl;
		return false;
	}

	pointLights.push_back(e);
	
	loadPointLights();

	return true;
}

bool Nova::Lighting::LightManager::addPointLight(Nova::Component::PointLight pl)
{
	return false;
}

void Nova::Lighting::LightManager::removePointLight(Nova::String name)
{
}

void Nova::Lighting::LightManager::removePointLight(Nova::Entity e)
{
}

void Nova::Lighting::LightManager::loadPointLight(Nova::Entity e)
{
	//Get the index of the point light in the array, and the point light info
	auto i = std::distance(pointLights.begin(), std::find(pointLights.begin(), pointLights.end(), e));
	auto plInfo = e.get<Nova::Component::PointLight>();
	auto transform = e.get<Nova::Component::Transform>();

	std::string str = "pointLights[";
	str += std::to_string(i);
	str += "].";

	for (auto program : programs)
	{
		glProgramUniform3fv(program, glGetUniformLocation(program, (str + "pos").c_str()), 1, transform->position.data());

		glProgramUniform3fv(program, glGetUniformLocation(program, (str + "ambient").c_str()), 1, plInfo->base.ambient.data());
		glProgramUniform3fv(program, glGetUniformLocation(program, (str + "diffuse").c_str()), 1, plInfo->base.diffuse.data());
		glProgramUniform3fv(program, glGetUniformLocation(program, (str + "specular").c_str()), 1, plInfo->base.specular.data());

		glProgramUniform1f(program, glGetUniformLocation(program, (str + "constant").c_str()), plInfo->constant);
		glProgramUniform1f(program, glGetUniformLocation(program, (str + "linear").c_str()), plInfo->linear);
		glProgramUniform1f(program, glGetUniformLocation(program, (str + "quadratic").c_str()), plInfo->quadratic);
	}
}

Nova::UInt Nova::Lighting::LightManager::getPointLightCount()
{
	return pointLights.size();
}

void Nova::Lighting::LightManager::loadPointLights()
{
	for (auto program : programs)
	{
		for (int i = 0; i < pointLights.size(); ++i)
		{
			auto plInfo = pointLights[i].get<Nova::Component::PointLight>();
			auto transform = pointLights[i].get<Nova::Component::Transform>();

			std::string str = "pointLights[";
			str += std::to_string(i);
			str += "].";

			glProgramUniform3fv(program, glGetUniformLocation(program, (str + "pos").c_str()), 1, transform->position.data());

			glProgramUniform3fv(program, glGetUniformLocation(program, (str + "ambient").c_str()), 1, plInfo->base.ambient.data());
			glProgramUniform3fv(program, glGetUniformLocation(program, (str + "diffuse").c_str()), 1, plInfo->base.diffuse.data());
			glProgramUniform3fv(program, glGetUniformLocation(program, (str + "specular").c_str()), 1, plInfo->base.specular.data());

			glProgramUniform1f(program, glGetUniformLocation(program, (str + "constant").c_str()), plInfo->constant);
			glProgramUniform1f(program, glGetUniformLocation(program, (str + "linear").c_str()), plInfo->linear);
			glProgramUniform1f(program, glGetUniformLocation(program, (str + "quadratic").c_str()), plInfo->quadratic);
		}

		glProgramUniform1i(program, glGetUniformLocation(program, "nPointLights"), pointLights.size());
	}
}

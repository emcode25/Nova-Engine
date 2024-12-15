#ifndef SHADER_HPP
#define SHADER_HPP

#include "types.hpp"
#include "structs.hpp"

namespace Nova
{
	class Shader
	{
	public:
		//TODO: Implement big three
		Shader();

		Nova::Int init(Nova::String vertexFilename, Nova::String fragmentFilename);
		Nova::Int init(Nova::String vertexFilename, Nova::String geometryFilename, Nova::String fragmentFilename);
		Nova::Int compileShaders();

		~Shader();

		Nova::UInt getProgram(void);

	private:
		Nova::UInt program;
		Nova::ShaderInfo info;
	};

	class ShaderManager
	{
	public:
		ShaderManager();

		bool addShader(Nova::Shader& shader);
		void recompileShaders();

		Nova::Array<Nova::ShaderProgram> getShaderPrograms();

	private:
		Nova::Array<std::reference_wrapper<Nova::Shader>> shaders;
	};
}

#endif
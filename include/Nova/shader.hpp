#ifndef SHADER_HPP
#define SHADER_HPP

#include "types.hpp"

namespace Nova
{
	class Shader
	{
	public:
		Shader();

		Nova::Int init(Nova::String vertexFilename, Nova::String fragmentFilename);
		Nova::Int init(Nova::String vertexFilename, Nova::String geometryFilename, Nova::String fragmentFilename);

		~Shader();

		Nova::UInt getProgram(void);

	private:
		Nova::UInt program;
	};
}

#endif
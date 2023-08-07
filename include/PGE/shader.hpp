#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>

#include <string>

namespace PGE
{
	class Shader
	{
	public:
		Shader(const char* vertexFilename, const char* fragmentFilename);

		GLuint getProgram(void);

	private:
		GLuint program;
	};
}

#endif
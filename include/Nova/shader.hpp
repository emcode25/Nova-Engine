#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>

namespace Nova
{
	class Shader
	{
	public:
		Shader();

		int init(const char* vertexFilename, const char* fragmentFilename);

		~Shader();

		GLuint getProgram(void);

	private:
		GLuint program;
	};
}

#endif
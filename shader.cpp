#include <PGE/shader.hpp>

#include <iostream>

#include <PGE/utils.hpp>

PGE::Shader::Shader(const char* vertexFilename, const char* fragmentFilename)
{
	const char *vertexCode, *fragmentCode;
    std::string temp = readFileToString(vertexFilename);
    vertexCode = temp.c_str();
    temp = readFileToString(fragmentFilename);
    fragmentCode = temp.c_str();

    //Create shaders
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexCode, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentCode, NULL);
    glCompileShader(fragmentShader);

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

GLuint PGE::Shader::getProgram(void)
{
	return program;
}
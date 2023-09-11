#include <Nova/shader.hpp>

#include <iostream>

#include <Nova/utils.hpp>

void checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

Nova::Shader::Shader()
{
    program = (GLuint)-1; //Max uint possible
}

//Init is necessary because shader creation needs to be after OpenGL initializes,
//so initialization must wait until after that occurs.
int Nova::Shader::init(const char* vertexFilename, const char* fragmentFilename)
{
    std::string vertexCode = readFileToString(vertexFilename);
    std::string fragmentCode = readFileToString(fragmentFilename);
    const char* temp;

    //Create shaders
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    temp = vertexCode.c_str();
    glShaderSource(vertexShader, 1, &temp, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    temp = fragmentCode.c_str();
    glShaderSource(fragmentShader, 1, &temp, NULL);
    glCompileShader(fragmentShader);

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return 0;
}

Nova::Shader::~Shader()
{
    glDeleteProgram(program);
}

GLuint Nova::Shader::getProgram(void)
{
	return program;
}
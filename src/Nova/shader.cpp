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
    //TODO: Make program an optional value
    program = static_cast<GLuint>(-1); //Max uint possible
    info = {};
}

//Init is necessary because shader creation needs to be after OpenGL initializes,
//so initialization must wait until after that occurs.
Nova::Int Nova::Shader::init(Nova::String vertexFilename, Nova::String fragmentFilename)
{
    info.vertexPath = vertexFilename;
    info.geometryPath = "";
    info.fragmentPath = fragmentFilename;

    compileShaders();

    return 0;
}

Nova::Int Nova::Shader::init(Nova::String vertexFilename, Nova::String geometryFilename, Nova::String fragmentFilename)
{
    info.vertexPath = vertexFilename;
    info.geometryPath = geometryFilename;
    info.fragmentPath = fragmentFilename;

    compileShaders();

    return 0;
}

Nova::Int Nova::Shader::compileShaders()
{
    const char* temp;
    GLuint vertexShader, geometryShader, fragmentShader;
    program = glCreateProgram();

    std::cout << "Starting shader compilation for files: " << std::endl
        << "\tVertex:   " << info.vertexPath << std::endl
        << "\tGeometry: " << info.geometryPath << std::endl
        << "\tFragment: " << info.fragmentPath << std::endl;

    //Create shaders
    std::string vertexCode = readFileToString(info.vertexPath);
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    temp = vertexCode.c_str();
    glShaderSource(vertexShader, 1, &temp, NULL);
    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader, "SHADER");
    glAttachShader(program, vertexShader);

    if (!info.geometryPath.empty())
    {
        std::string geometryCode = readFileToString(info.geometryPath);
        geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        temp = geometryCode.c_str();
        glShaderSource(geometryShader, 1, &temp, NULL);
        glCompileShader(geometryShader);
        checkCompileErrors(geometryShader, "SHADER");
        glAttachShader(program, geometryShader);
    }

    std::string fragmentCode = readFileToString(info.fragmentPath);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    temp = fragmentCode.c_str();
    glShaderSource(fragmentShader, 1, &temp, NULL);
    glCompileShader(fragmentShader);
    checkCompileErrors(fragmentShader, "SHADER");
    glAttachShader(program, fragmentShader);
   

    glLinkProgram(program);
    checkCompileErrors(program, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    if (!info.geometryPath.empty())
    {
        glDeleteShader(geometryShader);
    }

    std::cout << "Completed shader compilation." << std::endl;

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

Nova::ShaderManager::ShaderManager() {}

bool Nova::ShaderManager::addShader(Nova::Shader& shader)
{
    shaders.push_back(shader);

    return true;
}

void Nova::ShaderManager::recompileShaders()
{
    for (auto shaderRef : shaders)
    {
        auto& shader = shaderRef.get();

        glDeleteProgram(shader.getProgram());
        shader.compileShaders();
    }
}

Nova::Array<Nova::ShaderProgram> Nova::ShaderManager::getShaderPrograms()
{
    Nova::Array<Nova::ShaderProgram> programs;

    for (auto shaderRef : shaders)
    {
        auto& shader = shaderRef.get();
        programs.push_back(shader.getProgram());
    }

    return programs;
}

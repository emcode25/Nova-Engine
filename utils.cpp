#include <PGE/utils.hpp>

#include <fstream>
#include <sstream>

std::string PGE::readFileToString(const char* filename)
{
	std::ifstream reader(filename);
	std::ostringstream stream;
	stream << reader.rdbuf();
	reader.close();

	return stream.str();
}

void PGE::processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}
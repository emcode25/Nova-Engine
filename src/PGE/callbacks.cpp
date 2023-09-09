#include <glad/glad.h>
#include <PGE/callbacks.hpp>

void PGE::resizeCB(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
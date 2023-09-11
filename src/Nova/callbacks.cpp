#include <glad/glad.h>
#include <Nova/callbacks.hpp>

void Nova::resizeCB(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
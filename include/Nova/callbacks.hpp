#ifndef CALLBACKS_HPP
#define CALLBACKS_HPP

#include <GLFW/glfw3.h>

namespace Nova
{
    void resizeCB(GLFWwindow* window, int width, int height);
    void mouseCB(GLFWwindow* window, double xpos, double ypos);
}

#endif
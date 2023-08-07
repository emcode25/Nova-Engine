#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>

#include <GLFW/glfw3.h>

namespace PGE
{
    void processInput(GLFWwindow* window);
    std::string readFileToString(const char* filename);
}

#endif
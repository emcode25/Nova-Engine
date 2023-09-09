#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>

#include <GLFW/glfw3.h>
#include <Eigen/Core>
#include <Eigen/Geometry>

#include <PGE/structs.hpp>
#include <PGE/components.hpp>

namespace PGE
{
    void processInput(GLFWwindow* window);
    std::string readFileToString(const char* filename);

    PGE::Texture loadTexture(const char* filename, PGE::TexType type);

    Eigen::Matrix4f lookAt(const Eigen::Vector3f& position, const Eigen::Vector3f& target, const Eigen::Vector3f& up = {0.0f, 1.0f, 0.0f});
    Eigen::Matrix4f makePerspective(float aspectRatio, float fov, float near, float far);

    //This function assumes angles are in radians
    Eigen::Quaternionf rotateFromEuler(Eigen::Vector3f angles, bool isRadians = false);
}

#endif
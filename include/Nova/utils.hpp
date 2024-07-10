#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <memory>

#include <GLFW/glfw3.h>
#include <Eigen/Core>
#include <Eigen/Geometry>

#include <Nova/structs.hpp>
#include <Nova/components.hpp>
#include <Nova/editor_camera.hpp>

namespace Nova
{
    void processInput(GLFWwindow* window, Nova::Editor::EditorCamera& cam, float dt);
    std::string readFileToString(const char* filename);

    std::shared_ptr<Nova::Texture> loadTexture(const char* filename, Nova::TexType type);

    Eigen::Matrix4f lookAt(const Eigen::Vector3f& position, const Eigen::Vector3f& target, const Eigen::Vector3f& up = {0.0f, 1.0f, 0.0f});
    Eigen::Matrix4f makePerspective(float aspectRatio, float fov, float near, float far);

    //This function assumes angles are in degrees, second parameter should be true if already in radians
    Eigen::Quaternionf rotateFromEuler(Eigen::Vector3f angles, bool isRadians = false);
}

#endif
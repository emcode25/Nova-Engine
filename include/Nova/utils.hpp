#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <memory>
#include <string>

#include <GLFW/glfw3.h>
#include <Eigen/Core>
#include <Eigen/Geometry>

#include <Nova/types.hpp>
#include <Nova/structs.hpp>
#include <Nova/components.hpp>
#include <Nova/editor_camera.hpp>

namespace Nova
{
    void processInput(GLFWwindow* window, Nova::Editor::EditorCamera& cam, Nova::Float dt);
    Nova::String readFileToString(Nova::String filename);

    Nova::MeshInfo findMesh(const Nova::String& name, const Nova::Array<MeshInfo>& meshes);
    void deleteMeshes(Nova::Array<Nova::MeshInfo>& meshes);

    Nova::TextureInfo* loadTexture(Nova::String name, Nova::String filename, Nova::TexType type, Nova::Array<Nova::TextureInfo*>& textureSet);
    Nova::TextureInfo* findTexture(const Nova::String& name, const Nova::Array<TextureInfo*>& textureSet);
    void deleteTextures(Nova::Array<Nova::TextureInfo*>& textureSet);

    Nova::Matrix4 lookAt(const Nova::Vector3& position, const Nova::Vector3& target, const Nova::Vector3& up = {0.0f, 1.0f, 0.0f});
    Nova::Matrix4 makePerspective(Nova::Float aspectRatio, Nova::Float fov, Nova::Float near, Nova::Float far);

    //This function assumes angles are in degrees, second parameter should be true if already in radians
    Nova::Quaternion rotateFromEuler(Nova::Vector3 angles, bool isRadians = false);

    Nova::Int saveScene(const Nova::String& filepath);
    Nova::Int loadScene(const Nova::String& filepath);
    void clearScene();
}

#endif
#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <vector>

#include <glad/glad.h>

#include <Eigen/Core>

#include <Nova/structs.hpp>

namespace Nova
{
    struct Transform
    {
        Eigen::Vector3f position;
        Eigen::Vector3f rotation;
        Eigen::Vector3f scale;
    };

    const Transform DEFAULT_TRANSFORM =
    {
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f},
        {1.0f, 1.0f, 1.0f}
    };

    struct Mesh
    {
        std::vector<Vertex>  vertices;
        std::vector<GLuint>  indices;
        std::vector<Texture> textures;
        GLuint VAO;
        GLuint VBO;
        GLuint EBO;
    };

    struct Camera
    {
        float fov;
        float zNear;
        float zFar;
    };
}

#endif
#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <Eigen/Core>
#include <glad/glad.h>
#include <vector>

namespace PGE
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

    //Vertex is meant to be used to populate the Mesh component
    struct Vertex
    {
        Eigen::Vector3f pos;
        Eigen::Vector3f normal;
        Eigen::Vector2f uv;
    };

    struct Texture
    {

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
}

#endif
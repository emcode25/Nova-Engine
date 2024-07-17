#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <vector>
#include <memory>

#include <glad/glad.h>

#include <Eigen/Core>

#include <Nova/structs.hpp>

namespace Nova
{
    namespace Component
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
            std::vector<Texture*> textures;
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

        struct DirectionalLight
        {
            BaseLight base;
            Eigen::Vector3f direction;
        };

        const Camera DEFAULT_CAMERA = {45.0f, 1.0f, 100.0f};
    }
}

#endif
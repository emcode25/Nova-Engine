#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <vector>
#include <memory>

#include <glad/glad.h>

#include <Eigen/Core>

#include <Nova/types.hpp>
#include <Nova/structs.hpp>

namespace Nova
{
    namespace Component
    {
        struct Transform
        {
            Nova::Vector3 position;
            Nova::Vector3 rotation;
            Nova::Vector3 scale;
        };

        const Transform DEFAULT_TRANSFORM =
        {
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {1.0f, 1.0f, 1.0f}
        };

        struct Mesh
        {
            Nova::Array<Vertex>  vertices;
            Nova::Array<UInt>  indices;
            Nova::Array<Texture*> textures;
            Nova::UInt VAO;
            Nova::UInt VBO;
            Nova::UInt EBO;
        };

        struct Camera
        {
            Nova::Float fov;
            Nova::Float zNear;
            Nova::Float zFar;
        };

        struct DirectionalLight
        {
            Nova::BaseLight base;
            Nova::Vector3 direction;
        };

        struct PointLight
        {
            Nova::BaseLight base;

            Nova::Float constant;
            Nova::Float linear;
            Nova::Float quadratic;
        };

        const Camera DEFAULT_CAMERA = { 45.0f, 1.0f, 100.0f };
    }
}

#endif
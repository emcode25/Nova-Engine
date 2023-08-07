#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <Eigen/Core>

namespace PGE
{
    struct Transform
    {
        Eigen::Vector3f position;
        Eigen::Vector3f rotation;
        Eigen::Vector3f scale;
    };

    struct Mesh
    {
        GLfloat* vertices;
        GLuint VAO;
        GLuint VBO;
    };
}

#endif
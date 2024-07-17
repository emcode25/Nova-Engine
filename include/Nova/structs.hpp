#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include <string>

#include <Nova/enums.hpp>

namespace Nova
{
    //Vertex is meant to be used to populate the Mesh component
    struct Vertex
    {
        Eigen::Vector3f pos;
        Eigen::Vector3f normal;
        Eigen::Vector2f uv;
    };

    struct Texture
    {
        unsigned int texture;
        std::string name;
        Nova::TexType type;
    };

    struct BaseLight
    {
        Eigen::Vector3f ambient;
        Eigen::Vector3f diffuse;
        Eigen::Vector3f specular;
    };
}

#endif
#ifndef STRUCTS_HPP
#define STRUCTS_HPP

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
        Nova::TexType type;
    };
}

#endif
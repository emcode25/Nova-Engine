#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include <string>

#include <Nova/types.hpp>
#include <Nova/enums.hpp>

namespace Nova
{
    //Vertex is meant to be used to populate the Mesh component
    struct Vertex
    {
        Nova::Vector3 pos;
        Nova::Vector3 normal;
        Nova::Vector2 uv;
    };

    struct Texture
    {
        Nova::UInt texture;
        Nova::String name;
        Nova::TexType type;
    };

    struct BaseLight
    {
        Nova::Vector3 ambient;
        Nova::Vector3 diffuse;
        Nova::Vector3 specular;
    };

    struct ShaderInfo
    {
        Nova::String vertexPath;
        Nova::String geometryPath;
        Nova::String fragmentPath;
    };
}

#endif
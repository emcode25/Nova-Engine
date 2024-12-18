#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include <string>

#include <Nova/types.hpp>
#include <Nova/enums.hpp>

namespace Nova
{
    //VertexData is the layout of per-vertex data
    struct VertexData
    {
        Nova::Vector3 pos;
        Nova::Vector3 normal;
        Nova::Vector2 uv;
    };

    //MeshInfo contains the needed info per mesh
    struct MeshInfo
    {
        Nova::UInt VAO;
        Nova::UInt indexCount;
        Nova::String name;
        Nova::String filepath;
    };

    struct TextureInfo
    {
        Nova::UInt texture;
        Nova::String name;
        Nova::String path;
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
#ifndef CONST_HPP
#define CONST_HPP

#include "types.hpp"

namespace Nova
{
    namespace CONST
    {
        constexpr Nova::Int OPENGL_VERSION_MAJOR = 4;
        constexpr Nova::Int OPENGL_VERSION_MINOR = 6;
        constexpr Nova::Int OPENGL_SHADER_TEXTURE_MAX = 16;

        constexpr Nova::Int SCREEN_WIDTH = 800;
        constexpr Nova::Int SCREEN_HEIGHT = 600;

        constexpr Nova::Float DEG_TO_RAD = 3.1415927f / 180.0f;
        constexpr Nova::Float RAD_TO_DEG = 180.0f / 3.1415927f;

        constexpr Nova::Int OBJECT_NAME_CHARACTER_LIMIT = 256;

        constexpr Nova::Int MAX_POINT_LIGHTS = 10;
        constexpr Nova::Int MAX_SPOT_LIGHTS = 3;
    }
}

#endif
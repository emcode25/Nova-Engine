#ifndef CONST_HPP
#define CONST_HPP

namespace Nova
{
    namespace CONST
    {
        constexpr int OPENGL_VERSION_MAJOR = 4;
        constexpr int OPENGL_VERSION_MINOR = 6;
        constexpr int OPENGL_SHADER_TEXTURE_MAX = 16;

        constexpr int SCREEN_WIDTH = 800;
        constexpr int SCREEN_HEIGHT = 600;

        constexpr float DEG_TO_RAD = 3.1415927f / 180.0f;
        constexpr float RAD_TO_DEG = 180.0f / 3.1415927f;

        constexpr int OBJECT_NAME_CHARACTER_LIMIT = 256;

        constexpr int MAX_POINT_LIGHTS = 10;
        constexpr int MAX_SPOT_LIGHTS = 3;
    }
}

#endif
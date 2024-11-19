#ifndef UI_HPP
#define UI_HPP

#include <Nova/types.hpp>

#include <GLFW/glfw3.h>

#include <flecs.h>

namespace Nova
{
    namespace EditorUI
    {
        void MainMenu(GLFWwindow* window, const flecs::world& ecs, Nova::Array<Nova::Entity>& objs);
        void ShowObjectProperties(Nova::Entity& obj);
        void ShowObjectList(Nova::Array<Nova::Entity>& objs, Nova::Entity& activeObj);
    }
}

#endif
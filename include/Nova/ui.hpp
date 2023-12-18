#ifndef UI_HPP
#define UI_HPP

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <flecs.h>

namespace Nova
{
    namespace EditorUI
    {
        void MainMenu(GLFWwindow* window, const flecs::world& ecs, std::vector<flecs::entity>& objs);
        void ShowObjectProperties(flecs::entity& obj);
        void ShowObjectList(std::vector<flecs::entity>& objs, flecs::entity& activeObj);
    }
}

#endif
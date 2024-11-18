#ifndef ENGINE_HPP

#include <vector>
#include "components.hpp"
#include "shader.hpp"
#include "editor_camera.hpp"

namespace Nova
{
    //Records the time passed since the last frame
    extern float deltaTime;

    //Records the time up to the last frame, to be used for deltaTime calculations
    extern float lastTime;

    //The flecs world object that stores all information about every object and component
    extern flecs::world ecs;

    //The context to apply graphics processes
    extern GLFWwindow* window;

    //Global ID list of all entities
    extern std::vector<flecs::entity> entities;

    //Global store for all textures
    extern std::vector<Nova::Texture*> globalTextures;

    //The shader to use without lighting
    extern Nova::Shader unlitShader;

    //The shader to apply editor effects
    extern Nova::Shader forwardShader;

    //The shader to apply to light sources
    extern Nova::Shader lightSourceShader;

    //The shader for highlighting the active object
    extern Nova::Shader activeObjShader;

    //The camera that the editor uses, not a part of the final game
    extern Nova::Editor::EditorCamera editorCamera;

    //The object to highlight when editing the game
    extern flecs::entity activeObj;

    //The shader program to use when rendering
    extern GLuint activeProgram;
}

#endif
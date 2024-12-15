#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <vector>
#include "types.hpp"
#include "components.hpp"
#include "shader.hpp"
#include "editor_camera.hpp"
#include "light_manager.hpp"

namespace Nova
{
    //Records the time passed since the last frame
    extern Nova::Float deltaTime;

    //Records the time up to the last frame, to be used for deltaTime calculations
    extern Nova::Float lastTime;

    //The flecs world object that stores all information about every object and component
    extern Nova::World ecs;

    //The context to apply graphics processes
    extern GLFWwindow* window;

    //Global ID list of all entities
    extern Nova::Array<Nova::Entity> entities;

    //Global store for all textures
    extern Nova::Array<Nova::Texture*> globalTextures;

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
    extern Nova::Entity activeObj;

    //The shader program to use when rendering
    extern Nova::ShaderProgram activeProgram;

    //The lighting manager in charge of all light sources
    extern Nova::Lighting::LightManager lightManager;

    //The shader manager to handle recompiles
    extern Nova::ShaderManager shaderManager;
}

#endif
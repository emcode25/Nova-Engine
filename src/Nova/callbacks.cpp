#include <glad/glad.h>
#include <Nova/callbacks.hpp>

#include <Nova/editor_camera.hpp>

void Nova::resizeCB(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Nova::mouseCB(GLFWwindow* window, double xpos, double ypos)
{
    static float prevX = 0;
    static float prevY = 0;
    static bool held = false;

    float xposf = static_cast<float>(xpos);
    float yposf = static_cast<float>(ypos);

    if(!held && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))
    {
        prevX = xposf;
        prevY = yposf;
        held = true;
    } 
    
    if(held)
    {
        if(!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))
        {
            held = false;
        }

        float xoff = xposf - prevX;
        float yoff = prevY - yposf;

        prevX = xposf;
        prevY = yposf;

        //Cheat a little here, oops
        extern Nova::Editor::EditorCamera editorCamera;
        editorCamera.updateDirection(xoff, yoff);
    }
}
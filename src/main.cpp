#include <iostream>
#include <string>
#include <cmath>
#include <chrono>
#include <thread>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <flecs.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <Nova/const.hpp>
#include <Nova/utils.hpp>
#include <Nova/callbacks.hpp>
#include <Nova/components.hpp>
#include <Nova/objects.hpp>
#include <Nova/shader.hpp>
#include <Nova/ui.hpp>
#include <Nova/editor_camera.hpp>

std::vector<flecs::entity> cubes;

Eigen::Vector3f cubePositions[] = {
    { 0.0f,  0.0f,  0.0f},
    { 2.0f,  5.0f, -15.0f},
    {-1.5f, -2.2f, -2.5f},
    {-3.8f, -2.0f, -12.3f},
    { 2.4f, -0.4f, -3.5f},
    {-1.7f,  3.0f, -7.5f},
    { 1.3f, -2.0f, -2.5f},
    { 1.5f,  2.0f, -2.5f},
    { 1.5f,  0.2f, -1.5f},
    {-1.3f,  1.0f, -1.5f}
};

namespace Nova
{
    float deltaTime = 0.0f;
    float lastTime = 0.0f;

    flecs::world ecs;
    GLFWwindow* window;

    std::vector<flecs::entity> entities; //Global id list of all entities
    std::vector<Nova::Texture> globalTextures;
    Nova::Shader defaultShader;
    Nova::Shader activeObjShader;
    Nova::Editor::EditorCamera editorCamera;
    flecs::entity activeObj;

    int initGraphics(void)
    {
        //---------------GLFW---------------//
        //Begin GLFW
        glfwInit();

        //Set hints
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, Nova::CONST::OPENGL_VERSION_MAJOR);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, Nova::CONST::OPENGL_VERSION_MINOR);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        //Create and set window
        window = glfwCreateWindow(Nova::CONST::SCREEN_WIDTH, Nova::CONST::SCREEN_HEIGHT, "Nova Engine", NULL, NULL);
        if(window == NULL)
        {
            std::cerr << "Failed to create a GLFW window." << std::endl;
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);

        //Set up callbacks
        glfwSetFramebufferSizeCallback(window, Nova::resizeCB);


        //---------------GLAD---------------//
        //Initialize GLAD
        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cerr << "Failed to initialize GLAD." << std::endl;
            glfwTerminate();
            return -2;
        }


        //---------------OpenGL---------------//
        //Set the viewport
        glViewport(0, 0, Nova::CONST::SCREEN_WIDTH, Nova::CONST::SCREEN_HEIGHT);
        
        //Allow OpenGL settings
        glEnable(GL_DEPTH_TEST);

        //Increase lines thickness for highlighting objects
        glLineWidth(3.3f);

        //---------------IMGUI---------------//
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init();


        //---------------Nova---------------//
        //Create shader
        defaultShader.init("../shaders/vertex.vert", "../shaders/fragment.frag");
        activeObjShader.init("../shaders/active/vertex.vert", "../shaders/active/geometry.geom", "../shaders/active/fragment.frag");

        stbi_set_flip_vertically_on_load(true);
        
        return 0;
    }

    int initECS()
    {
        //Render system includes transformation information
        auto renderSystem = Nova::ecs.system<const Nova::Component::Transform, const Nova::Component::Mesh>("Render")
        .iter([](flecs::iter& it, const Nova::Component::Transform* t, const Nova::Component::Mesh* mesh)
        {
            static auto cam = editorCamera.getCameraProperties();

            //Window size must be accounted for as well due to resize
            int windowWidth = 0, windowHeight = 0;
            glfwGetFramebufferSize(window, &windowWidth, &windowHeight);

            //This rotation calculation is needed once per render system call 
            //Translation MUST ALSO BE APPLIED to work properly
            //TODO: Fix roll rotation on camera
            //Transform to view space
            Eigen::Matrix4f view = editorCamera.viewMatrix();

            //Project into clip space
            Eigen::Matrix4f proj = Nova::makePerspective(
                static_cast<float>(windowWidth) / static_cast<float>(windowHeight),
                cam.fov * Nova::CONST::DEG_TO_RAD, cam.zNear, cam.zFar); //Do not forget about integer division

            //Loop setup
            //Activate program and send matrices to shaders
            GLuint program = defaultShader.getProgram();
            glUseProgram(program);

            GLuint modelLoc = glGetUniformLocation(program, "model");
            GLuint viewLoc = glGetUniformLocation(program, "view");
            GLuint projLoc = glGetUniformLocation(program, "proj");

            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.data());
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, proj.data());

            for (auto i : it)
            {
                //Modify transform properties (locally)
                Nova::Component::Transform transform = t[i];

                //Perform world space transformations
                Eigen::Affine3f model = Eigen::Affine3f::Identity();
                model.translate(transform.position);
                model.rotate(Nova::rotateFromEuler(transform.rotation));
                model.scale(transform.scale);

                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.data());

                //Render the mesh
                glBindVertexArray(mesh[i].VAO);
                glDrawElements(GL_TRIANGLES, mesh[i].indices.size(), GL_UNSIGNED_INT, 0);
            }

            if(activeObj.has<Nova::Component::Mesh>())
            {
                //Get the active object and do the same transform stuff
                auto activeTransform = activeObj.get_ref<Nova::Component::Transform>();
                Eigen::Affine3f model = Eigen::Affine3f::Identity();
                model.translate(activeTransform->position);
                model.rotate(Nova::rotateFromEuler(activeTransform->rotation));
                model.scale(activeTransform->scale);

                //Send to active object shader
                GLuint program = activeObjShader.getProgram();
                glUseProgram(program);

                GLuint modelLoc = glGetUniformLocation(program, "model");
                GLuint viewLoc = glGetUniformLocation(program, "view");
                GLuint projLoc = glGetUniformLocation(program, "proj");

                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.data());
                glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.data());
                glUniformMatrix4fv(projLoc, 1, GL_FALSE, proj.data());

                //Render
                auto activeMesh = activeObj.get_ref<Nova::Component::Mesh>();
                glBindVertexArray(activeMesh->VAO);
                glDrawElements(GL_TRIANGLES, activeMesh->indices.size(), GL_UNSIGNED_INT, 0);
            }
        });

        //Create test cubes
        cubes.resize(10);
        for (int i = 0; i < 10; ++i)
        {
            Nova::Component::Transform transform;
            transform.position = cubePositions[i];
            transform.rotation = Eigen::Vector3f(0.8629051f, 0.2558994f, 0.4338701f).normalized() * 20.0f * i; 
            //TODO: Rotation about an arbitrary axis is broken
            transform.scale = Eigen::Vector3f(1.0f, 1.0f, 1.0f);

            std::string name = "Cube " + std::to_string(i);
            cubes[i] = Nova::createCube(Nova::ecs);
            cubes[i].set_doc_name(name.c_str());
            
            auto cubeTransform = cubes[i].set<Nova::Component::Transform>(transform);

            entities.push_back(cubes[i]);
        }

        auto cam = Nova::createCamera(ecs);
        cam.set_doc_name("Camera");
        entities.push_back(cam);

        activeObj = cam;

        return 0;
    }

    int mainLoop()
    {
        while(!glfwWindowShouldClose(window))
        {
            //Calculate delta-times
            float thisFrame = static_cast<float>(glfwGetTime());
            Nova::deltaTime = thisFrame - Nova::lastTime;
            Nova::lastTime  = thisFrame;

            //Get the inputs and process them
            glfwPollEvents();
            processInput(Nova::window, Nova::editorCamera, Nova::deltaTime);

            //Let ImGUI work
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::ShowDemoWindow();

            Nova::EditorUI::MainMenu(Nova::window, Nova::ecs, Nova::entities);
            
            Nova::EditorUI::ShowObjectProperties(activeObj);
            Nova::EditorUI::ShowObjectList(Nova::entities, activeObj);

            ImGui::Render();

            //Clean the slate
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //Run the systems and pipelines
            ecs.progress(Nova::deltaTime);

            //Swap buffers
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(window);

            //Framerate lock at 60fps, TESTING ONLY
            float remaining = static_cast<float>(glfwGetTime()) - Nova::deltaTime;
            if (remaining < (1.0f / 60.0f))
            {
                std::this_thread::sleep_for(
                    std::chrono::nanoseconds(
                        static_cast<long long>(remaining * 1000000000.0f)));
            }
        }

        return 0;
    }

    void quit()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(Nova::window);
        glfwTerminate();
    }
}

int main(void)
{
    Nova::initGraphics();
    
    Nova::initECS();

    Nova::mainLoop();

    for (auto cube : cubes)
    {
        cube.destruct();
    }

    Nova::quit();

    return 0;
}
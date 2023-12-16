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
    flecs::entity editorCamera;

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
        defaultShader.init("../../../shaders/vertex.vert", "../../../shaders/fragment.frag");

        stbi_set_flip_vertically_on_load(true);
        
        return 0;
    }

    int initECS()
    {
        //Render system includes transformation information
        auto renderSystem = Nova::ecs.system<const Transform, const Mesh>("Render")
        .iter([](flecs::iter& it, const Transform* t, const Mesh* mesh)
        {
            //Due to potential active camera change, properties must be retrieved once per render system call
            auto cam = editorCamera.get_ref<Nova::Camera>();
            auto camTransform = editorCamera.get_ref<Nova::Transform>();

            //Window size must be accounted for as well due to resize
            int windowWidth = 0, windowHeight = 0;
            glfwGetFramebufferSize(window, &windowWidth, &windowHeight);

            //This rotation calculation is needed once per render system call 
            //Translation MUST ALSO BE APPLIED to work properly
            //TODO: Fix roll rotation on camera
            Eigen::Vector3f camTarget = Nova::rotateFromEuler(camTransform->rotation) *
                Eigen::Vector3f(0.0f, 0.0f, -1.0f) + camTransform->position;

            //Transform to view space
            Eigen::Matrix4f view = Nova::lookAt(camTransform->position, camTarget);

            //Project into clip space
            Eigen::Matrix4f proj = Nova::makePerspective(
                static_cast<float>(windowWidth) / static_cast<float>(windowHeight),
                cam->fov * Nova::CONST::DEG_TO_RAD, cam->zNear, cam->zFar); //Do not forget about integer division

            for (auto i : it)
            {
                //Modify transform properties (locally)
                Nova::Transform transform = t[i];

                //Perform world space transformations
                Eigen::Affine3f model = Eigen::Affine3f::Identity();
                model.translate(transform.position);
                model.rotate(Nova::rotateFromEuler(transform.rotation));
                model.scale(transform.scale);

                //Activate program and send matrices to shaders
                GLuint program = defaultShader.getProgram();
                glUseProgram(program);

                GLuint modelLoc = glGetUniformLocation(program, "model");
                GLuint viewLoc = glGetUniformLocation(program, "view");
                GLuint projLoc = glGetUniformLocation(program, "proj");

                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.data());
                glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.data());
                glUniformMatrix4fv(projLoc, 1, GL_FALSE, proj.data());

                //Render the mesh
                glBindVertexArray(mesh[i].VAO);
                glDrawElements(GL_TRIANGLES, mesh[i].indices.size(), GL_UNSIGNED_INT, 0);
            }
        });

        //Set up the main camera as the active camera
        flecs::entity cam = ecs.entity("Editor Camera");
        cam.add<Nova::Transform>();
        cam.set<Nova::Transform>({{0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}});
        cam.add<Nova::Camera>();
        cam.set<Nova::Camera>({45.0f, 0.1f, 100.0f});
        editorCamera = cam;

        //Load and store the container texture
        globalTextures.push_back(Nova::loadTexture("../../../data/textures/container.jpg", Nova::TexType::DIFFUSE));

        //Create test cubes
        cubes.resize(10);
        for (int i = 0; i < 10; ++i)
        {
            Transform transform;
            transform.position = cubePositions[i];
            transform.rotation = Eigen::Vector3f(0.8629051f, 0.2558994f, 0.4338701f).normalized() * 20.0f * i; 
            //TODO: Rotation about an arbitrary axis is broken
            transform.scale = Eigen::Vector3f(1.0f, 1.0f, 1.0f);

            std::string name = "Cube " + std::to_string(i);
            cubes[i] = Nova::createCube(ecs);
            cubes[i].set_doc_name(name.c_str());
            
            auto cubeTransform = cubes[i].set<Nova::Transform>(transform);

            auto cubeMesh = cubes[i].get_ref<Nova::Mesh>();
            cubeMesh->textures.push_back(globalTextures[0]);

            entities.push_back(cubes[i]);
        }

        return 0;
    }

    int mainLoop()
    {
        while(!glfwWindowShouldClose(window))
        {
            //The active object to focus on
            static flecs::entity activeObj = entities[0];

            //Calculate delta-times
            float thisFrame = static_cast<float>(glfwGetTime());
            Nova::deltaTime = thisFrame - Nova::lastTime;
            Nova::lastTime  = thisFrame;

            //Get the inputs and process them
            glfwPollEvents();
            processInput(Nova::window);

            //Grab necessary references
            auto camProps = editorCamera.get_ref<Nova::Camera>();

            //Let ImGUI work
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            //TODO: MainMenu function
            {
                bool quit = false;

                if (ImGui::BeginMainMenuBar())
                {
                    if (ImGui::BeginMenu("File"))
                    {
                        //TODO: Implement a save system
                        ImGui::MenuItem("Save",  "Ctrl-S");
                        ImGui::MenuItem("Save as...", "Ctrl-Shift-S");

                        ImGui::Separator();
                        ImGui::MenuItem("Quit", "Alt-F4", &quit);

                        ImGui::EndMenu();
                    }

                    if (ImGui::BeginMenu("Edit"))
                    {
                        ImGui::EndMenu();
                    }

                    if (ImGui::BeginMenu("Object"))
                    {
                        if (ImGui::BeginMenu("New Object"))
                        {
                            bool newCube = false;
                            ImGui::MenuItem("Cube", NULL, &newCube);

                            if (newCube)
                            {
                                auto cube = Nova::createCube(Nova::ecs);
                                cube.set_doc_name("Cube");
                                
                                auto cubeMesh = cube.get_ref<Nova::Mesh>();
                                cubeMesh->textures.push_back(globalTextures[0]);

                                entities.push_back(cube);
                            }

                            ImGui::EndMenu();
                        }

                        ImGui::EndMenu();
                    }
                    
                    ImGui::EndMainMenuBar();
                }
                
                if (quit)
                {
                    glfwSetWindowShouldClose(window, quit);
                }
            }

            //ImGui suggests that a separate code block is helpful
            //TODO: Move this to a Camera property section in ShowObjectProperties
            {
                ImGui::Begin("Camera Controls");

                ImGui::SliderFloat("FOV", &(camProps->fov), 1.0f, 45.0f, "%.1f");
                ImGui::DragFloat("Near", &(camProps->zNear), 0.1f, 0.0f, 0.0f, "%.1f");
                ImGui::DragFloat("Far", &(camProps->zFar), 0.1f, 0.0f, 0.0f, "%.1f");

                ImGui::End();
            }

            //TODO: ShowObjectProperties function
            auto activeTransform = activeObj.get_ref<Nova::Transform>();
            {
                ImGui::Begin("Object Transform");

                ImGui::DragFloat3("Position", &(activeTransform->position(0)), 0.05f);
                ImGui::DragFloat3("Rotation", &(activeTransform->rotation(0)), 1.0f);
                ImGui::DragFloat3("Scale", &(activeTransform->scale(0)), 0.1f);

                ImGui::End();
            }

            ImGui::ShowDemoWindow();
            //TODO: ShowObjectList function
            {
                ImGui::Begin("Object List", NULL, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);
                
                static std::string name;
                if (ImGui::BeginListBox("Objects", ImGui::GetWindowContentRegionMax()))
                {
                    for (auto e : entities)
                    {
                        const bool selected = (activeObj == e);
                        name = e.doc_name();
                        name += "##";
                        name += e.raw_id();
                        if (ImGui::Selectable(name.c_str(), selected))
                        {
                            activeObj = e;
                        }

                        if (selected)
                        {
                            ImGui::SetItemDefaultFocus();
                        }

                        if (ImGui::BeginPopupContextItem())
                        {
                            
                            ImGui::Text("Rename:");
                            ImGui::InputText("##rename", name.c_str(), IM_ARRAYSIZE(name));
                            if (ImGui::Button("Close"))
                                ImGui::CloseCurrentPopup();
                            ImGui::EndPopup();
                        }
                        //TODO: Allow renaming of objects
                    }

                    ImGui::EndListBox();
                }
                
                ImGui::End();
            }

            ImGui::Render();

            //Clean the slate
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //All logic here

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
    Nova::editorCamera.destruct();

    Nova::quit();

    return 0;
}
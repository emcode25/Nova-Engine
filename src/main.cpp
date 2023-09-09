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

#include <PGE/const.hpp>
#include <PGE/utils.hpp>
#include <PGE/callbacks.hpp>
#include <PGE/components.hpp>
#include <PGE/objects.hpp>
#include <PGE/shader.hpp>

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

namespace PGE
{
    float deltaTime = 0.0f;
    float lastTime = 0.0f;

    flecs::world ecs;
    GLFWwindow* window;

    std::vector<flecs::entity> entities; //Global id list of all entities
    std::vector<PGE::Texture> globalTextures;
    PGE::Shader defaultShader;
    flecs::entity editorCamera;

    int initGraphics(void)
    {
        //---------------GLFW---------------//
        //Begin GLFW
        glfwInit();

        //Set hints
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, PGE::CONST::OPENGL_VERSION_MAJOR);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, PGE::CONST::OPENGL_VERSION_MINOR);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        //Create and set window
        window = glfwCreateWindow(PGE::CONST::SCREEN_WIDTH, PGE::CONST::SCREEN_HEIGHT, "PGE", NULL, NULL);
        if(window == NULL)
        {
            std::cerr << "Failed to create a GLFW window." << std::endl;
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);

        //Set up callbacks
        glfwSetFramebufferSizeCallback(window, PGE::resizeCB);


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
        glViewport(0, 0, PGE::CONST::SCREEN_WIDTH, PGE::CONST::SCREEN_HEIGHT);
        
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


        //---------------PGE---------------//
        //Create shader
        defaultShader.init("../../../shaders/vertex.vert", "../../../shaders/fragment.frag");

        stbi_set_flip_vertically_on_load(true);
        
        return 0;
    }

    int initECS()
    {
        //Render system includes transformation information
        auto renderSystem = PGE::ecs.system<const Transform, const Mesh>("Render")
        .iter([](flecs::iter& it, const Transform* t, const Mesh* mesh)
        {
            //Due to potential active camera change, properties must be retrieved once per render system call
            auto cam = editorCamera.get_ref<PGE::Camera>();
            auto camTransform = editorCamera.get_ref<PGE::Transform>();

            //Window size must be accounted for as well due to resize
            int windowWidth = 0, windowHeight = 0;
            glfwGetFramebufferSize(window, &windowWidth, &windowHeight);

            //This rotation calculation is needed once per render system call 
            //Translation MUST ALSO BE APPLIED to work properly
            //TODO: Fix roll rotation on camera
            Eigen::Vector3f camTarget = PGE::rotateFromEuler(camTransform->rotation) *
                Eigen::Vector3f(0.0f, 0.0f, -1.0f) + camTransform->position;

            for (auto i : it)
            {
                //Modify transform properties (locally)
                PGE::Transform transform = t[i];

                //Perform world space transformations
                Eigen::Affine3f model = Eigen::Affine3f::Identity();
                model.translate(transform.position);
                model.rotate(PGE::rotateFromEuler(transform.rotation));
                model.scale(transform.scale);

                //Transform to view space
                Eigen::Matrix4f view = PGE::lookAt(camTransform->position, camTarget);

                //Project into clip space
                Eigen::Matrix4f proj = PGE::makePerspective(
                    static_cast<float>(windowWidth) / static_cast<float>(windowHeight),
                    cam->fov * PGE::CONST::DEG_TO_RAD, cam->zNear, cam->zFar); //Do not forget about integer division

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
        cam.add<PGE::Transform>();
        cam.set<PGE::Transform>({{0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}});
        cam.add<PGE::Camera>();
        cam.set<PGE::Camera>({45.0f, 0.1f, 100.0f});
        editorCamera = cam;

        //Load and store the container texture
        globalTextures.push_back(PGE::loadTexture("../../../data/textures/container.jpg", PGE::TexType::DIFFUSE));

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
            cubes[i] = PGE::createCube(ecs);
            cubes[i].set_doc_name(name.c_str());
            
            auto cubeTransform = cubes[i].set<PGE::Transform>(transform);

            auto cubeMesh = cubes[i].get_ref<PGE::Mesh>();
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
            PGE::deltaTime  = thisFrame - PGE::lastTime;
            PGE::lastTime   = thisFrame;

            //Get the inputs and process them
            glfwPollEvents();
            processInput(PGE::window);

            //Grab necessary references
            auto camProps = editorCamera.get_ref<PGE::Camera>();

            //Let ImGUI work
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            //ImGui suggests that a separate code block is helpful
            {
                ImGui::Begin("Camera Controls");

                ImGui::SliderFloat("FOV", &(camProps->fov), 1.0f, 45.0f, "%.1f");
                ImGui::DragFloat("Near", &(camProps->zNear), 0.1f, 0.0f, 0.0f, "%.1f");
                ImGui::DragFloat("Far", &(camProps->zFar), 0.1f, 0.0f, 0.0f, "%.1f");

                ImGui::End();
            }

            auto activeTransform = activeObj.get_ref<PGE::Transform>();
            {
                ImGui::Begin("Object Transform");

                ImGui::DragFloat3("Position", &(activeTransform->position(0)), 0.05f);
                ImGui::DragFloat3("Rotation", &(activeTransform->rotation(0)), 1.0f);
                ImGui::DragFloat3("Scale", &(activeTransform->scale(0)), 0.1f);

                ImGui::End();
            }

            //ImGui::ShowDemoWindow();
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
            ecs.progress(PGE::deltaTime);

            //Swap buffers
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(window);

            //Framerate lock at 60fps, TESTING ONLY
            float remaining = static_cast<float>(glfwGetTime()) - PGE::deltaTime;
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

        glfwDestroyWindow(PGE::window);
        glfwTerminate();
    }
}

int main(void)
{
    PGE::initGraphics();

    PGE::initECS();

    PGE::mainLoop();

    for (auto cube : cubes)
    {
        cube.destruct();
    }
    PGE::editorCamera.destruct();

    PGE::quit();

    return 0;
}
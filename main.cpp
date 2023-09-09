#include <iostream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

flecs::entity cube;

namespace PGE
{
    flecs::world ecs;
    GLFWwindow* window;

    std::vector<PGE::Texture> globalTextures;
    PGE::Shader defaultShader;
    flecs::entity activeCamera;

    int initGraphics(void)
    {
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

        //Initialize GLAD
        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cerr << "Failed to initialize GLAD." << std::endl;
            glfwTerminate();
            return -2;
        }

        //Set the viewport
        glViewport(0, 0, PGE::CONST::SCREEN_WIDTH, PGE::CONST::SCREEN_HEIGHT);

        //Set up callbacks
        glfwSetFramebufferSizeCallback(window, PGE::resizeCB);
        
        //Allow OpenGL settings
        glEnable(GL_DEPTH_TEST);

        //Create shader
        defaultShader.init("../../../shaders/vertex.vert", "../../../shaders/fragment.frag");
        
        return 0;
    }

    int initECS()
    {
        //Render system includes transformation information
        auto renderSystem = PGE::ecs.system<const Transform, const Mesh>("Render")
            .each([](flecs::entity e, const Transform& t, const Mesh& mesh)
            {
                //Camera only needs its info pulled once
                static auto cam = activeCamera.get_ref<PGE::Camera>();
                static auto camTransform = activeCamera.get_ref<PGE::Transform>();
                static Eigen::Vector3f camTarget = 
                    PGE::rotateFromEuler(camTransform->rotation).normalized().toRotationMatrix() * 
                    Eigen::Vector3f(0.0f, 0.0f, -1.0f);
                
                //Modify transform properties (locally)
                PGE::Transform transform = t;

                //Perform world space transformations
                Eigen::Affine3f model = Eigen::Affine3f::Identity();
                model.translate(transform.position);
                model.rotate(PGE::rotateFromEuler(transform.rotation, true));
                model.scale(transform.scale);

                //Transform to view space
                Eigen::Matrix4f view = PGE::lookAt(camTransform->position, camTarget);

                //Project into clip space
                Eigen::Matrix4f proj = PGE::makePerspective(
                    static_cast<float>(PGE::CONST::SCREEN_WIDTH) / static_cast<float>(PGE::CONST::SCREEN_HEIGHT), 
                    cam->fov * PGE::CONST::DEG_TO_RAD, cam->zNear, cam->zFar); //Do not forget about integer division
                
                //Activate program and send matrices to shaders
                GLuint program = defaultShader.getProgram();
                glUseProgram(program);

                GLuint modelLoc = glGetUniformLocation(program, "model");
                GLuint viewLoc  = glGetUniformLocation(program, "view");
                GLuint projLoc  = glGetUniformLocation(program, "proj");
                
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.data());
                glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.data());
                glUniformMatrix4fv(projLoc, 1, GL_FALSE, proj.data());

                //Render the mesh
                glBindVertexArray(mesh.VAO);
                glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
            });

        //Set up the main camera as the active camera
        flecs::entity cam = ecs.entity("Main Camera");
        cam.add<PGE::Transform>();
        cam.set<PGE::Transform>({{0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}});
        cam.add<PGE::Camera>();
        cam.set<PGE::Camera>({45.0f, 0.1f, 100.0f});
        activeCamera = cam;

        //Load and store the container texture
        globalTextures.push_back(PGE::loadTexture("../../../data/textures/container.jpg", PGE::TexType::DIFFUSE));

        //Create a test cube
        cube = PGE::createCube(ecs);
        cube.set_name("cube1");
        auto cubeMesh = cube.get_ref<PGE::Mesh>();
        cubeMesh->textures.push_back(globalTextures[0]);

        return 0;
    }

    int mainLoop()
    {
        while(!glfwWindowShouldClose(window))
        {
            //Clean the slate
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //Process new inputs
            processInput(PGE::window);

            //All logic here
            const Transform* oldTransform = cube.get<PGE::Transform>();
            Transform transform = *oldTransform;
            transform.rotation = Eigen::Vector3f(0.0f, static_cast<float>(glfwGetTime()), 0.0f);

            cube.set<PGE::Transform>(transform);
            
            //Run the systems and pipelines
            ecs.progress();

            //Swap buffers and get new inputs
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        return 0;
    }

    void quit()
    {
        glfwDestroyWindow(PGE::window);
        glfwTerminate();
    }
}

int main(void)
{
    stbi_set_flip_vertically_on_load(true);

    PGE::initGraphics();

    PGE::initECS();

    const auto *t = cube.get<PGE::Mesh>();

    std::cout << "Pos: " << t->VAO << std::endl;
    std::cout << "Rot: " << t->VBO << std::endl;

    PGE::mainLoop();

    cube.destruct();

    PGE::quit();

    return 0;
}
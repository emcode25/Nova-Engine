#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <flecs.h>

#include <iostream>
#include <cmath>

#include <PGE/const.hpp>
#include <PGE/utils.hpp>
#include <PGE/callbacks.hpp>
#include <PGE/components.hpp>
#include <PGE/objects.hpp>
#include <PGE/shader.hpp>

flecs::entity cube;
float triangleVertices[] = 
{
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

namespace PGE
{
    Shader defaultShader;
    flecs::world ecs;
    GLFWwindow* window;

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
                //Modify transform properties (locally)
                PGE::Transform transform = t;
                transform.rotation = Eigen::Vector3f(0.0f, static_cast<float>(glfwGetTime()), 0.0f);

                //Perform world space transformations
                Eigen::Affine3f model = Eigen::Affine3f::Identity();
                model.translate(transform.position);
                model.rotate(PGE::rotateFromEuler(transform.rotation, true));
                model.scale(transform.scale);

                //Transform to view space
                Eigen::Matrix4f view = PGE::lookAt({0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, 0.0f});

                //Project into clip space
                Eigen::Matrix4f proj = PGE::makePerspective(
                    static_cast<float>(PGE::CONST::SCREEN_WIDTH) / static_cast<float>(PGE::CONST::SCREEN_HEIGHT), 
                    45.0f * PGE::CONST::DEG_TO_RAD, 0.1f, 100.0f); //Do not forget about integer division
                
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
                glBindVertexArray(mesh.VAO);
                glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
            });

        //tri = PGE::ecs.entity(); //Creates a new entity
    
        //Add a transform to the triangle.
        //tri.add<PGE::Transform>();
        //tri.set<PGE::Transform>({{1.0f, 2.0f, 3.0f}, {0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}});

        //GLuint tempVAO, tempVBO;
        //glGenBuffers(1, &tempVBO);
        //glGenVertexArrays(1, &tempVAO);
        //tri.set<PGE::Mesh>({triangleVertices, tempVAO, tempVBO});

        //glBindVertexArray(tempVAO);
        //glBindBuffer(GL_ARRAY_BUFFER, tempVBO);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
        //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
        //glEnableVertexAttribArray(0);

        cube = PGE::createCube(ecs);
        cube.set_name("cube1");

        return 0;
    }

    int mainLoop()
    {
        while(!glfwWindowShouldClose(window))
        {
            //Process new inputs
            processInput(PGE::window);

            //All logic here
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //const Mesh* tempMesh = cube.get<Mesh>();
            //glUseProgram(defaultShader.getProgram());
            //glBindVertexArray(tempMesh->VAO);
            //glDrawArrays(GL_TRIANGLES, 0, 36);

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
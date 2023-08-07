#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Eigen/Core>
#include <flecs.h>

#include <iostream>

#include <PGE/const.hpp>
#include <PGE/utils.hpp>
#include <PGE/callbacks.hpp>
#include <PGE/components.hpp>

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

    
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

GLuint shaderProgram;
flecs::entity tri;
float triangleVertices[] = 
{
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

namespace PGE
{
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

        //Create shaders
        GLuint vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        
        GLuint fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        
        return 0;
    }

    int initECS()
    {
        tri = PGE::ecs.entity(); //Creates a new entity
    
        //Add a transform to the triangle.
        tri.add<PGE::Transform>();
        tri.set<PGE::Transform>({{1.0f, 2.0f, 3.0f}, {0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}});

        GLuint tempVAO, tempVBO;
        glGenBuffers(1, &tempVBO);
        glGenVertexArrays(1, &tempVAO);
        tri.set<PGE::Mesh>({triangleVertices, tempVAO, tempVBO});

        glBindVertexArray(tempVAO);
        glBindBuffer(GL_ARRAY_BUFFER, tempVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);

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
            glClear(GL_COLOR_BUFFER_BIT);

            const Mesh* tempMesh = tri.get<Mesh>();
            glUseProgram(shaderProgram);
            glBindVertexArray(tempMesh->VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            //Swap buffers and get new inputs
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        return 0;
    }

    void quit()
    {
        glfwTerminate();
    }
}

int main(void)
{
    PGE::initGraphics();

    PGE::initECS();

    const PGE::Transform *t = tri.get<PGE::Transform>();

    std::cout << "Pos: " << t->position.transpose() << std::endl;
    std::cout << "Rot: " << t->rotation.transpose() << std::endl;
    std::cout << "Sca: " << t->scale.transpose() << std::endl;

    PGE::mainLoop();

    tri.destruct();

    PGE::quit();

    return 0;
}
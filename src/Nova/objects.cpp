#include <Nova/objects.hpp>

#include <glad/glad.h>

#include <Nova/components.hpp>
#include <Nova/utils.hpp>

#define NOVA_HPP_NO_HEADERS
#include <Nova/nova.hpp>
#undef NOVA_HPP_NO_HEADERS

constexpr int NUM_CUBE_VERTICES = 17;
Nova::Vertex cubeVertices[] = 
{   //Position              Normals       UVs
    {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, //0
    {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}, //1
    {{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}, //2
    {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}, //3
    {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, //4
    {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}, //5
    {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}, //6
    {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}, //7
    {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}, //8
    {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}, //9
    {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}, //10
    {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}, //11
    {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}, //12
    {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, //13
    {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}, //14
    {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}  //15
};

GLuint cubeIndices[] =
{
    0, 1, 2,
    2, 3, 0,
    4, 5, 6,
    6, 7, 4,
    8, 9, 10,
    10, 4, 8,
    11, 2, 12,
    12, 13, 11,
    10, 14, 5,
    5, 4, 10,
    3, 2, 11,
    11, 15, 3
};

flecs::entity Nova::createCube(const flecs::world& ecs)
{
    //Cube texture
    static bool textureAdded = false;
    static Nova::Texture* containerTexture;

    if (!textureAdded)
    {
        containerTexture = Nova::loadTexture("Cube", "../../../data/textures/container.jpg", Nova::TexType::DIFFUSE, globalTextures);
        textureAdded = true;
    }

    //Create cube and add basic Transform
	flecs::entity cube = ecs.entity();
	cube.add<Nova::Component::Transform>();
	cube.set<Nova::Component::Transform>(Nova::Component::DEFAULT_TRANSFORM);

    //Load and set mesh for cube
    GLuint VAO, VBO, EBO;

    Nova::Component::Mesh mesh;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //Set mesh properties for use in GL commands
    mesh.vertices = std::vector<Nova::Vertex>(std::begin(cubeVertices), std::end(cubeVertices));
    mesh.indices  = std::vector<GLuint>(std::begin(cubeIndices), std::end(cubeIndices));
    mesh.textures = std::vector<Nova::Texture*>();
    mesh.textures.push_back(containerTexture);
    mesh.VAO = VAO;
    mesh.VBO = VBO;
    mesh.EBO = EBO;

    //Bind relevant objects
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    //Set data in buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(Nova::Vertex) * mesh.vertices.size(), &mesh.vertices[0], GL_STATIC_DRAW);

    //Position attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Nova::Vertex), (void*)offsetof(Nova::Vertex, pos));
    glEnableVertexAttribArray(0);

    //Normal attributes
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Nova::Vertex), (void*)offsetof(Nova::Vertex, normal));
    glEnableVertexAttribArray(1);

    //UV attributes
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Nova::Vertex), (void*)offsetof(Nova::Vertex, uv));
    glEnableVertexAttribArray(2);

    //Set indices
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh.indices.size(), &mesh.indices[0], GL_STATIC_DRAW);

	cube.add<Nova::Component::Mesh>();
    cube.set<Nova::Component::Mesh>(mesh);

    return cube;
}

flecs::entity Nova::createCamera(const flecs::world& ecs)
{
    flecs::entity cam = ecs.entity();
	cam.add<Nova::Component::Transform>();
	cam.set<Nova::Component::Transform>(Nova::Component::DEFAULT_TRANSFORM);
    cam.add<Nova::Component::Camera>();
    cam.set<Nova::Component::Camera>(Nova::Component::DEFAULT_CAMERA);

    //TODO: Add a camera mesh

    return cam;
}
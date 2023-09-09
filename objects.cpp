#include <PGE/objects.hpp>

#include <glad/glad.h>

#include <PGE/components.hpp>
#include <PGE/utils.hpp>

constexpr int NUM_CUBE_VERTICES = 17;
PGE::Vertex cubeVertices[] = 
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
    5, 14, 10,
    3, 2, 11,
    11, 15, 3
};

flecs::entity PGE::createCube(const flecs::world& ecs)
{
    //Create cube and add basic Transform
	flecs::entity cube = ecs.entity();
	cube.add<PGE::Transform>();
	cube.set<PGE::Transform>(PGE::DEFAULT_TRANSFORM);

    //Load and set mesh for cube
    GLuint VAO, VBO, EBO;

    PGE::Mesh mesh;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //Set mesh properties for use in GL commands
    mesh.vertices = std::vector<PGE::Vertex>(std::begin(cubeVertices), std::end(cubeVertices));
    mesh.indices  = std::vector<GLuint>(std::begin(cubeIndices), std::end(cubeIndices));
    mesh.textures = std::vector<PGE::Texture>();
    mesh.VAO = VAO;
    mesh.VBO = VBO;
    mesh.EBO = EBO;

    //Bind relevant objects
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    //Set data in buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(PGE::Vertex) * mesh.vertices.size(), &mesh.vertices[0], GL_STATIC_DRAW);

    //Position attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PGE::Vertex), (void*)offsetof(PGE::Vertex, pos));
    glEnableVertexAttribArray(0);

    //Normal attributes
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(PGE::Vertex), (void*)offsetof(PGE::Vertex, normal));
    glEnableVertexAttribArray(1);

    //UV attributes
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(PGE::Vertex), (void*)offsetof(PGE::Vertex, uv));
    glEnableVertexAttribArray(2);

    //Set indices
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh.indices.size(), &mesh.indices[0], GL_STATIC_DRAW);

	cube.add<PGE::Mesh>();
    cube.set<PGE::Mesh>(mesh);

    return cube;
}
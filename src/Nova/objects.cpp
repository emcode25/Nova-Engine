#include <Nova/objects.hpp>

#include <glad/glad.h>

#include <vector>
#include <Nova/types.hpp>
#include <Nova/components.hpp>
#include <Nova/utils.hpp>

#define NOVA_HPP_NO_HEADERS
#include <Nova/nova.hpp>
#undef NOVA_HPP_NO_HEADERS

constexpr Nova::UInt NUM_CUBE_VERTICES = 36;
Nova::Vertex cubeVertices[] = 
{   //Position              Normals                UVs
    {{-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}},
    {{ 0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 0.0f}},
    {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f}},
    {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f}},
    {{-0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 1.0f}},
    {{-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}},

    {{-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 0.0f}},
    {{ 0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 0.0f}},
    {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f}},
    {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f}},
    {{-0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 1.0f}},
    {{-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 0.0f}},

    {{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}},
    {{-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}},
    {{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}},
    {{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}},
    {{-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
    {{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}},

    {{ 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}},
    {{ 0.5f,  0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}},
    {{ 0.5f, -0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}},
    {{ 0.5f, -0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}},
    {{ 0.5f, -0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
    {{ 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}},

    {{-0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 1.0f}},
    {{ 0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f}},
    {{ 0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 0.0f}},
    {{ 0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 0.0f}},
    {{-0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 0.0f}},
    {{-0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 1.0f}},

    {{-0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {0.0f, 1.0f}},
    {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f}},
    {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 0.0f}},
    {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 0.0f}},
    {{-0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {0.0f, 0.0f}},
    {{-0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {0.0f, 1.0f}}
};

Nova::UInt cubeIndices[] =
{
    0, 1, 2,
    3, 4, 5,
    6, 7, 8,
    9, 10, 11,
    12, 13, 14,
    15, 16, 17,
    18, 19, 20,
    21, 22, 23,
    24, 25, 26,
    27, 28, 29,
    30, 31, 32,
    33, 34, 35
};

Nova::Entity Nova::createCube()
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
	Nova::Entity cube = Nova::ecs.entity();
	cube.add<Nova::Component::Transform>();
	cube.set<Nova::Component::Transform>(Nova::Component::DEFAULT_TRANSFORM);

    //Load and set mesh for cube
    Nova::UInt VAO, VBO, EBO;
    //TODO: only allocate VAO, VBO, and EBO once for all cubes
    Nova::Component::Mesh mesh;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //Set mesh properties for use in GL commands
    mesh.vertices = std::vector<Nova::Vertex>(std::begin(cubeVertices), std::end(cubeVertices));
    mesh.indices  = std::vector<Nova::UInt>(std::begin(cubeIndices), std::end(cubeIndices));
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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Nova::UInt) * mesh.indices.size(), &mesh.indices[0], GL_STATIC_DRAW);

	cube.add<Nova::Component::Mesh>();
    cube.set<Nova::Component::Mesh>(mesh);

    return cube;
}

flecs::entity Nova::createCamera()
{
    flecs::entity cam = Nova::ecs.entity();
	cam.add<Nova::Component::Transform>();
	cam.set<Nova::Component::Transform>(Nova::Component::DEFAULT_TRANSFORM);
    cam.add<Nova::Component::Camera>();
    cam.set<Nova::Component::Camera>(Nova::Component::DEFAULT_CAMERA);

    //TODO: Add a camera mesh

    return cam;
}

flecs::entity Nova::createLightCube(const Nova::Component::PointLight& props)
{
    flecs::entity cubeLight = ecs.entity();
    cubeLight.add<Nova::Component::Transform>();
    cubeLight.set<Nova::Component::Transform>(Nova::Component::DEFAULT_TRANSFORM);
    cubeLight.add<Nova::Component::PointLight>();
    cubeLight.set<Nova::Component::PointLight>(props);

    //Load and set mesh for cube
    GLuint VAO, VBO, EBO;

    Nova::Component::Mesh mesh;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //Set mesh properties for use in GL commands
    mesh.vertices = std::vector<Nova::Vertex>(std::begin(cubeVertices), std::end(cubeVertices));
    mesh.indices = std::vector<GLuint>(std::begin(cubeIndices), std::end(cubeIndices));
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

    cubeLight.add<Nova::Component::Mesh>();
    cubeLight.set<Nova::Component::Mesh>(mesh);

    return cubeLight;
}

flecs::entity Nova::createDefaultPointLight()
{
    //Add a light source
    Nova::BaseLight plBase;
    plBase.ambient = Nova::Vector3(0.1f, 0.1f, 0.1f);
    plBase.diffuse = Nova::Vector3(1.0f, 1.0f, 1.0f);
    plBase.specular = Nova::Vector3(0.0f, 0.0f, 0.0f);

    Nova::Component::PointLight plProps;
    plProps.base = plBase;
    plProps.constant = 1.0f;
    plProps.linear = 0.09f;
    plProps.quadratic = 0.032f;

    return Nova::createLightCube(plProps);
}
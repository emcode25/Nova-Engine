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
Nova::VertexData cubeVertices[] = 
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

Nova::MeshInfo Nova::loadCubeMesh()
{
    Nova::MeshInfo meshInfo;

    //Load mesh for cube
    Nova::UInt VAO, VBO, EBO;
    Nova::Component::Mesh mesh;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //Set mesh properties for use in GL commands
    meshInfo.VAO = VAO;
    meshInfo.VBO = VBO;
    meshInfo.EBO = EBO;
    meshInfo.indexCount = 36; //Size of constant array
    meshInfo.name = "Cube Mesh";
    globalMeshes.push_back(meshInfo);

    //Bind relevant objects
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    //Set data in buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(Nova::VertexData) * 36, cubeVertices, GL_STATIC_DRAW);

    //Position attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Nova::VertexData), (void*)offsetof(Nova::VertexData, pos));
    glEnableVertexAttribArray(0);

    //Normal attributes
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Nova::VertexData), (void*)offsetof(Nova::VertexData, normal));
    glEnableVertexAttribArray(1);

    //UV attributes
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Nova::VertexData), (void*)offsetof(Nova::VertexData, uv));
    glEnableVertexAttribArray(2);

    //Set indices
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Nova::UInt) * 36, cubeIndices, GL_STATIC_DRAW);

    return meshInfo;
}

Nova::Entity Nova::createCube()
{
    //TODO: Remove this code eventually
    //Cube texture
    static bool textureAdded = false;
    static Nova::TextureInfo* containerTexture;

    if (!textureAdded)
    {
        containerTexture = Nova::loadTexture("Container", "../../../data/textures/container.jpg", Nova::TexType::DIFFUSE, globalTextures);
        textureAdded = true;
    }

    //Create cube and add basic Transform
	Nova::Entity cube = Nova::ecs.entity();
	cube.add<Nova::Component::Transform>();
	cube.set<Nova::Component::Transform>(Nova::Component::DEFAULT_TRANSFORM);

    //Get mesh for cube
    Nova::Component::Mesh mesh;
    mesh.textures = std::vector<Nova::TextureInfo*>();
    mesh.meshInfo = findMesh("Cube Mesh", Nova::globalMeshes);

    TextureInfo* tex = findTexture("Container", Nova::globalTextures);
    if (tex)
    {
        mesh.textures.push_back(tex);
    }

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

    //Get mesh for cube
    Nova::Component::Mesh mesh;
    mesh.textures = std::vector<Nova::TextureInfo*>();
    mesh.meshInfo = findMesh("Cube Mesh", Nova::globalMeshes);

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
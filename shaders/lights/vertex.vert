#version 460 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;

out vec3 FragPos;
out vec3 Normal;
out vec2 UV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
   FragPos = vec3(model * vec4(inPos, 1.0));
   Normal = mat3(tranpose(inverse(model))) * inNormal;
   UV = inUV;

   gl_Position = proj * view * vec4(FragPos, 1.0);
}
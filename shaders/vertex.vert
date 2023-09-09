#version 330 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec2 uv;

void main()
{
   gl_Position = proj * view * model * vec4(inPos, 1.0);
   uv = inUV;
}
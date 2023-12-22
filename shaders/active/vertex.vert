#version 330 core
layout (location = 0) in vec3 inPos;

uniform mat4 model;
uniform mat4 view;

void main()
{
   gl_Position = view * model * vec4(inPos, 1.0);
}
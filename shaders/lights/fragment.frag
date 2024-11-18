#version 460 core

//-------------VARIABLES-------------//
in vec3 FragPos;
in vec3 Normal;
in vec2 UV;

out vec4 FragColor;

//Vectors
uniform vec4 lightColor;

//Samplers
uniform sampler2D tex1;

void main()
{
	FragColor = lightColor;
}
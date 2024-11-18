#version 460 core

//-------------VARIABLES-------------//
out vec4 FragColor;

//Vectors
uniform vec3 lightColor;

void main()
{
	FragColor = vec4(lightColor, 1.0);
}
#version 460 core

//-------------VARIABLES-------------//
in vec3 FragPos;
in vec3 Normal;
in vec2 UV;

out vec4 FragColor;

uniform sampler2D tex0;

void main()
{
	FragColor = texture(tex0, UV);
}
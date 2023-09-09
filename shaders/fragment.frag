#version 330 core
in vec2 uv;

uniform sampler2D tex1;

out vec4 FragColor;

void main()
{
   FragColor = texture(tex1, uv);
}
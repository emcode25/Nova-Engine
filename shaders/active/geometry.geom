#version 460 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

uniform mat4 proj;

void drawLine(int index1, int index2)
{
    gl_Position = proj * gl_in[index1].gl_Position;
    EmitVertex();
    gl_Position = proj * gl_in[index2].gl_Position;
    EmitVertex();
    EndPrimitive();
}

void main()
{
    drawLine(0, 1);
    drawLine(1, 2);
    drawLine(2, 0);
}
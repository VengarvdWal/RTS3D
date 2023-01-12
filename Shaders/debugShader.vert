#version 300 es                          			
precision mediump float;

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vCol;

out vec4 color;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position = projection * view * vec4(vPos, 1.0);
    color = vec4(vCol, 1.0);
}
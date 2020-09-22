#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec3 offset;

out vec4 color;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
     float scale = 10.0f;
    gl_Position = projection * view * model * vec4((aPos+ offset),1.0f); 
    color = aColor;
}
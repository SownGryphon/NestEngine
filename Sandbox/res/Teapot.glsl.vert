@shader vert
#version 330 core

layout(location = 0) in vec4 pos;
layout(location = 1) in vec3 normal;

layout(location = 0) out vec3 modelNormal;

uniform mat4 u_transform = mat4(1);
uniform mat4 u_MVP = mat4(1);

void main()
{
	gl_Position = u_MVP * u_transform * pos;
	modelNormal = normalize((u_transform * vec4(normal, 0)).xyz);
}
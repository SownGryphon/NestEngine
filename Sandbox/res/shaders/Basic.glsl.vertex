#version 330 core

layout(location = 0) in vec4 coord;

uniform mat4 u_transform = mat4(1.f);
uniform mat4 u_MVP = mat4(1.f);

void main()
{
	gl_Position = u_MVP * u_transform * coord;
}
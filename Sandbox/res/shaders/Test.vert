#version 330 core

layout(location = 0) in vec4 coord;

uniform mat4 u_MVP;
uniform mat4 u_transform;

out vec2 pos;

void main()
{
	pos = coord.xy + 0.5;

	gl_Position = u_MVP * u_transform * coord;
}
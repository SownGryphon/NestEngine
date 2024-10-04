@shader fragment
#version 330 core

uniform vec4 u_color = vec4(1);

out vec4 color;

void main()
{
	color = u_color;
}
@shader vertex
#version 330 core

layout(location = 0) in vec4 pos;

out vec3 worldPos;

uniform mat4 u_MVP = mat4(1);
uniform mat4 u_transform = mat4(1);

void main()
{
	gl_Position = u_MVP * u_transform * pos;
	worldPos = pos.xyz;
}

@shader geom
#version 330 core

layout(lines) in;
layout(line_strip, max_vertices = 2) out;

in vec3 worldPos[];

out vec4 gs_color;

void main()
{
	gs_color = vec4(worldPos[1], 1);
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();
	EndPrimitive();
}

@shader frag
#version 330 core

in vec4 gs_color;

layout(location = 0) out vec4 color;

void main()
{
	color = gs_color;
}
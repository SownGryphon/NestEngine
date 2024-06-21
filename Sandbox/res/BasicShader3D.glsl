@shader vertex
#version 330 core

layout(location = 0) in vec4 pos;

out vec3 worldPos;

uniform mat4 u_transform = mat4(1);
uniform mat4 u_MVP = mat4(1);

void main()
{
	vec4 modelPos = u_transform * pos;
	gl_Position = u_MVP * modelPos;
	worldPos = modelPos.xyz;
}

@shader geom
#version 330 core

layout(triangles) in;
in vec3 worldPos[];

layout(location = 0, triangle_strip, max_vertices = 3) out;

out vec3 normal;

void main()
{
	normal = normalize(cross(worldPos[1] - worldPos[0], worldPos[2] - worldPos[0]));
	for (int i = 0; i < 3; ++i)
	{
		gl_Position =  gl_in[i].gl_Position;
		EmitVertex();
	}
	EndPrimitive();
}

@shader frag
#version 330 core

vec3 lightDir = normalize(vec3(3, -10, 1));

uniform float u_globalLight = 0.3;
uniform vec4 u_color = vec4(1);

in vec3 normal;
layout(location = 0) out vec4 color;

void main()
{
	float lightMult = u_globalLight - (1 - u_globalLight) * 0.5 * dot(normal, lightDir);
	color = vec4(u_color.xyz * lightMult, u_color.w);
	//color = vec4(normal, 1);
	//color = vec4(-normal, 1);
	//color = vec4(abs(normal), 1);
}
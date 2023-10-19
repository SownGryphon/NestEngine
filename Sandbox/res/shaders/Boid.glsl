#type vertex
#version 330 core

layout(location = 0) in vec4 pos;
layout(location = 1) in float rotation;

out VS_OUT
{
	float rotation;
} vs_out;

void main()
{
	gl_Position = pos;
	vs_out.rotation = rotation;
}

#type geometry
#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 3) out;

in VS_OUT
{
	float rotation;
} gs_in[];

vec2 boidVertices[3] = vec2[3](
	vec2(5, 0),
	vec2(-3, 3),
	vec2(-3, -3)
);

uniform mat4 u_MVP = mat4(1);

vec4 modelPoint(int index, mat2 rotationMat)
{
	return u_MVP * (vec4(rotationMat * boidVertices[index], 0, 0) + gl_in[0].gl_Position);
}

void main()
{
	mat2 rotationMat = mat2(
		cos(gs_in[0].rotation), -sin(gs_in[0].rotation),
		sin(gs_in[0].rotation), cos(gs_in[0].rotation)
	);

	gl_Position = modelPoint(0, rotationMat);
	EmitVertex();
	gl_Position = modelPoint(1, rotationMat);
	EmitVertex();
	gl_Position = modelPoint(2, rotationMat);
	EmitVertex();
	EndPrimitive();
}

#type fragment
#version 330 core

out vec4 color;

void main()
{
	color = vec4(1, 1, 1, 1);
}
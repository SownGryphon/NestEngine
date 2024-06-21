@shader frag
#version 330 core

vec3 lightDir = normalize(vec3(3, -10, 1));

uniform float u_globalLight = 0.3;
uniform vec4 u_color = vec4(1);

layout(location = 0) in vec3 normal;

layout(location = 0) out vec4 color;

void main()
{
	float lightMult = u_globalLight - (1 - u_globalLight) * 0.5 * dot(normal, lightDir);
	color = vec4(u_color.xyz * lightMult, u_color.w);
	//color = vec4(normal, 1);
	//color = vec4(-normal, 1);
	//color = vec4(abs(normal), 1);
}
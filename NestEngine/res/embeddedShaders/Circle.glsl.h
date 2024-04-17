R"(
//#shader vertex
#version 330 core

layout(location = 0) in vec4 pos;

uniform mat4 u_transform = mat4(1);
uniform mat4 u_MVP = mat4(1);

layout(location = 0) out vec2 uv;

void main()
{
	gl_Position = u_MVP * u_transform * pos;
	uv = pos.xy * 2;
}

//#shader fragment
#version 330 core

layout(location = 0) in vec2 uv;

uniform vec4 u_fillColor = vec4(1.f);
uniform vec4 u_outlineColor = vec4(1.f);
uniform vec2 u_outlineWidth = vec2(0.f);

layout(location = 0) out vec4 color;

void main()
{
	float mag = dot(uv, uv);
	if (mag > 1) discard;

	vec2 fillVec = uv / (1.f - u_outlineWidth);

	float isOutline = step(1.f, dot(fillVec, fillVec));
	color = mix(u_fillColor, u_outlineColor, isOutline);
}
)"
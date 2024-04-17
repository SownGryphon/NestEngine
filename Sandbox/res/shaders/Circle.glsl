//#shader vertex
#version 330 core

layout(location = 0) in vec4 pos;
out vec2 circleCoord;

uniform mat4 u_transform = mat4(1);
uniform mat4 u_MVP = mat4(1);

void main()
{
	gl_Position = u_MVP * u_transform * pos;
	circleCoord = pos.xy;
}

//#shader fragment
#version 330 core

in vec2 circleCoord;

uniform vec4 u_color;

layout(location = 0) out vec4 color;

void main()
{
	if (length(circleCoord) > 0.5)
		discard;

	color = u_color;
}
@shader vert
#version 330 core

in vec2 pos;
in vec2 uv;

out vec2 texUV;

uniform mat4 u_MVP = mat4(1.f);
uniform mat4 u_transform = mat4(1.f);

void main()
{
	gl_Position = u_MVP * u_transform * vec4(pos, 0, 1);
	texUV = uv;
}

@shader fragment
#version 330 core

in vec2 texUV;

uniform sampler2D u_texture;

out vec4 color;

void main()
{
	color = texture(u_texture, texUV);
}
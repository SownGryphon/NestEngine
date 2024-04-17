#version 330 core

in vec2 pos;

const int numCircles = 100;
uniform vec3 u_circles[numCircles];

void main()
{
	vec3 color = vec3(1.);
	for (int i = 0; i < numCircles; i++)
	{
		float d = length(pos - u_circles[i].xy) - u_circles[i].z;
		color *= step(0., d);
	}

	gl_FragColor = vec4(color, 1.);
}
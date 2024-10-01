@shader vertex
#version 430 core

layout(location = 0) in vec4 pos;
layout(location = 1) in uint inContourBegin;
layout(location = 2) in uint inNumPoints;

out uint contourBegin;
out uint numPoints;

void main()
{
	gl_Position = pos;
	contourBegin = inContourBegin;
	numPoints = inNumPoints;
}

@shader geom
#version 430 core

in uint contourBegin[];
in uint numPoints[];

struct FontPoint
{
	int packedPos;
	int flags;
};

layout(packed, binding = 1) buffer FontPoints
{
	FontPoint fontPoints[];
};

layout(points) in;
layout(line_strip, max_vertices = 256) out;

out vec4 vecCol;

vec4 unpackPos(int packedPos)
{
	int posX = packedPos & 0xffff;
	if ((posX & 0x8000) != 0)
	{
		posX |= 0xffff0000;
	}

	int posY = (packedPos >> 16) & 0xffff;
	if ((posY & 0x8000) != 0)
	{
		posY |= 0xffff0000;
	}

	return vec4(posX / 4000.0, posY / 4000.0, 0.0, 0.0);
}

void main()
{
	bool newContour = true;
	FontPoint contourBeginPoint;
	for (uint i = 0; i < numPoints[0]; ++i)
	{
		FontPoint currentPoint = fontPoints[contourBegin[0] + i];

		if (newContour)
		{
			contourBeginPoint = currentPoint;
			newContour = false;
		}

		if ((currentPoint.flags & 1) == 0)
		{
			vecCol = vec4(1.0, 0.0, 1.0, 1.0);
		}
		else
		{
			vecCol = vec4(1.0, 1.0, 0.0, 1.0);
		}

		gl_Position = unpackPos(currentPoint.packedPos) + gl_in[0].gl_Position;
		EmitVertex();

		if ((currentPoint.flags & 2) != 0)
		{
			newContour = true;

			if ((contourBeginPoint.flags & 1) == 0)
			{
				vecCol = vec4(1.0, 0.0, 0.0, 1.0);
			}
			else
			{
				vecCol = vec4(1.0);
			}
			gl_Position =  unpackPos(contourBeginPoint.packedPos) + gl_in[0].gl_Position;
			EmitVertex();
			EndPrimitive();
		}
	}
}

@shader frag
#version 430 core

in vec4 vecCol;

out vec4 color;

void main()
{
	color = vecCol;
}
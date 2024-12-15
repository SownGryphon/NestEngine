@shader vertex
#version 440 core

layout(location = 0) in vec4 pos;
layout(location = 1) in ivec2 inFPos;
layout(location = 2) in uint inContourBegin;
layout(location = 3) in uint inNumPoints;

out vec2 fPos;
out uint symbolBegin;
out uint numPoints;

uniform mat4 u_MVP;
uniform mat4 u_transform;

void main()
{
	gl_Position = u_MVP * u_transform * pos;
	fPos = inFPos;
	symbolBegin = inContourBegin;
	numPoints = inNumPoints;
}

@shader frag
#version 430 core

@neDefaultUniforms

uniform float u_fuPerPx = 16384.0;

in vec2 fPos;
flat in uint symbolBegin;
flat in uint numPoints;

int lastCrossDir = 0;
int firstCrossDir = -2;

vec2 checkPositions[3][2] = {
	{
		vec2(0.0, 0.0),
		vec2(0.0, 0.67)
	},
	{
		vec2(0.5, 0.17),
		vec2(0.5, 0.83)
	},
	{
		vec2(1.0, 0.33),
		vec2(1.0, 1.0)
	}
};

const float epsilon = 1e-4;

struct FontPoint
{
	vec2 pos;
	bool contourEnd;
};

readonly layout(packed, binding = 1) buffer FontPoints
{
	FontPoint fontPoints[];
};

out vec4 color;

bool equalEpsilon(float a, float b, float negBias, float posBias)
{
	return a >= b - negBias && a <= b + posBias;
}

int checkBezier(in vec2 fPoint, in vec2 pos1, in vec2 pos2, in vec2 control, bool lastContourSeg)
{
	vec2 v1 = pos1 - control,
		v2 = pos2 - control,
		vx = fPoint - control;

	vec2 a = v1 + v2;
	vec2 b = -2.0 * v1;
	vec2 c = v1 - vx;
		
	int res = 0;
	
	// Collinearity check
	if (a.y == 0.0)
	{
		if (v1.y == 0.0)
			return 0;

		if (vx.y > max(v1.y, v2.y))
			return 0;
		
		if (vx.y < min(v1.y, v2.y))
			return 0;

		float t = (vx.y - v1.y) / (v2.y - v1.y);
		float intersectionX = mix(v1.x, v2.x, t);

		if (vx.x > intersectionX)
			return 0;
			
		res = int(sign(v2.y - v1.y));
	}
	else
	{
		float intersectionT1 = (-b.y - sqrt(b.y * b.y - 4.0 * a.y * c.y)) / (2.0 * a.y);
		float intersectionT2 = (-b.y + sqrt(b.y * b.y - 4.0 * a.y * c.y)) / (2.0 * a.y);

		if (intersectionT1 >= 0.0 && intersectionT1 <= 1.0)
		{
			float intersectionX = (intersectionT1 * a.x + b.x) * intersectionT1 + c.y;

			if (vx.x <= intersectionX)
			{
				res += int(sign(2.0 a.y * intersectionT1 + b.y));
			}
		}

		if (intersectionT2 >= 0.0 && intersectionT2 <= 1.0)
		{
			float intersectionX = (intersectionT2 * a.x + b.x) * intersectionT2 + c.y;

			if (vx.x <= intersectionX)
			{
				res += int(sign(2.0 a.y * intersectionT2 + b.y));
			}
		}
	}

	if (vx.y == v1.y && res == lastCrossDir)
	{
		return 0;
	}

	if (lastContourSeg && vx.y == v2.y && res == firstCrossDir)
	{
		return 0;
	}

	lastCrossDir = res;

	if (firstCrossDir == -2 && vx.y == v2.y)
	{
		firstCrossDir = res;
	}

	return res;
}

bool isPointInGlyph(in ivec2 fPoint)
{
	int windingNum = 0;
	
	for (uint i = 2; i <= numPoints; i += 2)
	{
		FontPoint p1 = fontPoints[symbolBegin + i - 2];
		FontPoint control = fontPoints[symbolBegin + i - 1];
		FontPoint p2 = fontPoints[symbolBegin + i];
		ivec2 p1Pos = unpackPos(p1.packedPos);
		ivec2 controlPos = unpackPos(control.packedPos);
		ivec2 p2Pos = unpackPos(p2.packedPos);

		windingNum += checkBezier(fPoint, p1Pos, p2Pos, controlPos, p2.contourEnd);

		if (p2.contourEnd)
		{
			firstCrossDir = -2;
			lastCrossDir = 0;
			++i;
		}
	}

	return windingNum != 0;
}

void main()
{
	fPosInt = ivec2(fPos);

	uint coverage[3] = { 0, 0, 0 };
	uint coverageSum = 0;
	for (uint i = 0; i < 3; ++i)
	{
		for (uint j = 0; j < checkPositions[i].length(); ++j)
		{
			ivec2 fCheckPos = fPosInt + ivec2(checkPositions[i][j] * u_fuPerPx);
			if (isPointInGlyph(fCheckPos))
			{
				++coverage[i];
				++coverageSum;
			}
		}
	}

	if (coverageSum == 0)
	{
		discard;
	}

	#if 0
	if (windingNum < -1)
	{
		color.r = float(-windingNum + 1) / 2.0;
	}
	else if (windingNum > 0)
	{
		color.g = float(windingNum) / 2.0;
	}
	else
	{
		color = vec4(-firstCrossDir / 2.0, firstCrossDir / 2.0, 1.0, 1.0);
	}
	#else
	color.r = float(coverage[0]) / 2.0;
	color.g = float(coverage[1]) / 2.0;
	color.b = float(coverage[2]) / 2.0;
	color.w = float(coverageSum) / 6.0;
	#endif
}
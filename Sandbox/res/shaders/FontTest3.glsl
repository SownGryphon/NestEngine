@shader vertex
#version 440 core

layout(location = 0) in vec4 pos;
layout(location = 1) in vec2 inFPos;
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
	float x;
	float y;
	int flags;
};

readonly layout(std430, binding = 1) buffer FontPoints
{
	FontPoint fontPoints[];
};

out vec4 color;

bool equalEpsilon(float a, float b, float negBias, float posBias)
{
	return a >= b - negBias && a <= b + posBias;
}

bool closeToLine(in vec2 fPoint, in vec2 pos1, in vec2 pos2)
{
	vec2 lineVec = pos2 - pos1;
	vec2 posVec = fPoint - pos1;

	float t = dot(posVec, lineVec) / dot(lineVec, lineVec);
	t = clamp(t, 0.0, 1.0);

	if (length(fPoint - mix(pos1, pos2, t)) < u_fuPerPx)
		return true;
	return false;
}

int checkBezier(in vec2 fPoint, in vec2 pos1, in vec2 pos2, in vec2 control, bool lastContourSeg)
{
	float pointMaxX = max(max(pos1.x, pos2.x), control.x);
	float pointMinY = min(min(pos1.y, pos2.y), control.y);
	float pointMaxY = max(max(pos1.y, pos2.y), control.y);

	if (fPoint.x > pointMaxX
		|| fPoint.y < pointMinY
		|| fPoint.y > pointMaxY)
		return 0;

	vec2 a = pos1 + pos2 - 2.0 * control;
	vec2 b = 2.0 * (control - pos1);
	vec2 c = pos1 - fPoint;
	
	int res = 0;
	
	// Collinearity check
	if (a.y == 0.0)
	{
		if (b.y == 0.0)
			return 0;

		float t = (fPoint.y - pos1.y) / (pos2.y - pos1.y);
		float intersectionX = mix(pos1.x, pos2.x, t);

		if (fPoint.x > intersectionX)
			return 0;
			
		res = int(sign(pos2.y - pos1.y));
	}
	else
	{
		float disc = b.y * b.y - 4.0 * a.y * c.y;

		if (disc < 0.0)
			return 0;

		float intersectionT1 = (-b.y - sqrt(disc)) / (2.0 * a.y);
		float intersectionT2 = (-b.y + sqrt(disc)) / (2.0 * a.y);

		if (intersectionT1 >= 0.0 && intersectionT1 <= 1.0)
		{
			float intersectionX = (intersectionT1 * a.x + b.x) * intersectionT1 + c.x;

			if (intersectionX >= 0.0)
			{
				res += int(sign(2.0 * a.y * intersectionT1 + b.y));
			}
		}

		if (intersectionT2 >= 0.0 && intersectionT2 <= 1.0 && disc != 0.0)
		{
			float intersectionX = (intersectionT2 * a.x + b.x) * intersectionT2 + c.x;

			if (intersectionX >= 0.0)
			{
				res += int(sign(2.0 * a.y * intersectionT2 + b.y));
			}
		}
	}

	if (fPoint.y == pos1.y && res == lastCrossDir)
	{
		return 0;
	}

	if (lastContourSeg && fPoint.y == pos2.y && res == firstCrossDir)
	{
		return 0;
	}

	lastCrossDir = res;

	if (firstCrossDir == -2 && fPoint.y == pos2.y)
	{
		firstCrossDir = res;
	}

	return res;
}

bool isPointInGlyph(in vec2 fPoint)
{
	int windingNum = 0;

	int debugCount = 0;
	int debugTarget = -1;

	for (uint i = 2; i <= numPoints; i += 2)
	{
		FontPoint p1 = fontPoints[symbolBegin + i - 2];
		FontPoint control = fontPoints[symbolBegin + i - 1];
		FontPoint p2 = fontPoints[symbolBegin + i];
		vec2 p1Pos = vec2(p1.x, p1.y);
		vec2 controlPos = vec2(control.x, control.y);
		vec2 p2Pos = vec2(p2.x, p2.y);

		bool isLastContourPoint = (p2.flags & 0x2) == 0x2;

		windingNum += checkBezier(fPoint, p1Pos, p2Pos, controlPos, isLastContourPoint);

		if (isLastContourPoint)
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
	color = vec4(1.0);

	uint coverage[3] = { 0, 0, 0 };
	uint coverageSum = 0;
	#if 0
	for (uint i = 0; i < 3; ++i)
	{
		for (uint j = 0; j < checkPositions[i].length(); ++j)
		{
			vec2 fCheckPos = fPos + checkPositions[i][j] * u_fuPerPx;
			if (isPointInGlyph(fCheckPos))
			{
				++coverage[i];
				++coverageSum;
			}
		}
	}
	#else
	if (isPointInGlyph(fPos))
		coverageSum = 1;
	#endif

	if (coverageSum == 0)
	{
		//color.r = 1.0;
		discard;
		return;
	}

	return;

	color.r = float(coverage[0]) / 2.0;
	color.g = float(coverage[1]) / 2.0;
	color.b = float(coverage[2]) / 2.0;
	color.w = float(coverageSum);

	color = vec4(1.0);
}
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

in vec2 fPos;
flat in uint symbolBegin;
flat in uint numPoints;

ivec2 fPosInt;
int lastCrossDir = 0;
int firstCrossDir = -2;

const float epsilon = 1e-5;

struct FontPoint
{
	int packedPos;
	int flags;
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

ivec2 unpackPos(int packedPos)
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

	return ivec2(posX, posY);
}

int checkLine(in ivec2 pos1, in ivec2 pos2, bool lastContourSeg)
{
	ivec2 posDiff = pos2 - pos1;

	ivec2 crossVec = fPosInt - pos1;

	if (posDiff.y == 0 && crossVec.y == 0 && lastContourSeg && lastCrossDir == firstCrossDir)
	{
		return -firstCrossDir;
	}

	int result = sign(posDiff.y);
	crossVec.x -= crossVec.y * posDiff.x / posDiff.y;
	crossVec.y *= sign(posDiff.y);
	posDiff.y *= sign(posDiff.y);

	if (posDiff.y == 0)
	{
		if (firstCrossDir == -2)
		{
			firstCrossDir = 2;
		}

		return 0;
	}

	if (crossVec.y < 0 || crossVec.y > posDiff.y || crossVec.x > 0)
	{
		return 0;
	}

	if (crossVec.y == 0 && result == lastCrossDir)
		return 0;

	if (lastContourSeg && crossVec.y == posDiff.y && result == firstCrossDir)
		return 0;

	if (firstCrossDir == -2 && crossVec.y == 0)
		firstCrossDir = result;

	lastCrossDir = result;
	return result;
}

int checkBezier(in ivec2 pos1, in ivec2 pos2, in ivec2 control, bool lastContourSeg)
{
	// TODO: Add a collinearity check

	#if 1
	ivec2 v1 = pos1 - control,
		v2 = pos2 - control,
		vx = fPosInt - control;

	if (v1.y + v2.y != 0)
	{
		float k = -float(v1.x + v2.x) / float(v1.y + v2.y);

		mat2 skew = mat2(1.0, 0.0, k, sign(v1.y + v2.y));

		v1 = ivec2(skew * v1);
		v2 = ivec2(skew * v2);
		vx = ivec2(skew * vx);

		int endYMin = min(v1.y, v2.y),
			endYMax = max(v1.y, v2.y);

		if (vx.x > max(v1.x, v2.x))
		{
			return 0;
		}

		if (vx.y > endYMax)
		{
			return 0;
		}

		ivec2 a = v1 + v2,
			b = -2 * v1,
			c = v1;

		float t = float(vx.x - c.x) / float(b.x);
		t = clamp(t, 0.0, 1.0);

		float bezierY = t * (t * a.y + b.y) + c.y;

		int endSlope = sign(v2.y - v1.y) * sign(v2.x - v1.x);
		
		int res = 0;

		if (vx.y >= endYMin && (vx.y - bezierY) * endSlope >= 0)
		{
			int intersection = int(sign(pos2.y - pos1.y));
			res += intersection;

			if (vx.y == v1.y && intersection == lastCrossDir)
			{
				res -= intersection;
			}

			if (lastContourSeg && vx.y == v2.y && intersection == firstCrossDir)
			{
				res -= intersection;
			}
		}

		if (vx.y < endYMin && vx.y > bezierY)
		{
			color.g = 1.0;
			res += int(sign(pos1.x - pos2.x));
		}
		
		lastCrossDir = res;

		if (firstCrossDir == -2 && vx.y == v1.y)
		{
			firstCrossDir = res;
		}

		return res;
	}

	if (vx.y < min(v1.y, v2.y) || vx.y > max(v1.y, v2.y))
	{
		return 0;
	}

	float t = (vx.y - v1.y) / (-2.0 * v1.y);

	float bezierX = t * (t * (v1.x + v2.x) - 2.0 * v1.x) + v1.x;

	if (vx.x > bezierX)
	{
		return 0;
	}

	int res = int(sign(v2.y - v1.y));

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

	#endif

	#if 0
	// Old code

	int aY = pos1.y - 2 * control.y + pos2.y;
	int bY = 2 * control.y - 2 * pos1.y;
	int cY = pos1.y - fPosInt.y;

	int disc = bY * bY - 4 * aY * cY;
	if (disc < 0)
		return 0;

	float t0 = 0.0,
		t1 = 0.0;

	if (aY == 0)
	{
		t0 = -float(cY) / float(bY);
		t1 = -1.0;
	}
	else
	{
		t0 = (-bY - sqrt(disc)) / aY / 2.0;
		t1 = (-bY + sqrt(disc)) / (2.0 * aY);
	}

	int result = 0;

	if (disc == 0)
	{
		if ((equalEpsilon(t0, 0.0, 0.0, epsilon) || equalEpsilon(t0, 1.0, epsilon, 0.0)) && fPos.x < mix(pos1.x, pos2.x, t0))
		{
			int d2y = sign(aY);
			result = d2y * int(sign(0.5 - t0));
		}

		if (result == lastCrossDir)
			return 0;

		lastCrossDir = result;
		return result;
	}

	if (t0 >= 0.0 && t0 <= 1.0)
	{
		float x0 = control.x + pow(1.0 - t0, 2.0) * (pos1.x - control.x) + pow(t0, 2.0) * (pos2.x - control.x);

		if (x0 >= fPos.x)
		{
			float dy0 = 2.0 * (t0 * aY + bY);

			int cross0 = int(sign(dy0));

			if ((!equalEpsilon(t0, 0.0, 0.0, epsilon) || cross0 != lastCrossDir) && (!lastContourSeg || !equalEpsilon(t0, 1.0, epsilon, 0.0) || cross0 != firstCrossDir))
			{
				result += cross0;
				lastCrossDir = cross0;

				if (firstCrossDir == 0 && equalEpsilon(t0, 0.0, 0.0, epsilon))
					firstCrossDir = cross0;
			}
		}
	}

	if (t1 >= 0.0 && t1 <= 1.0)
	{
		float x1 = control.x + pow(1.0 - t1, 2.0) * (pos1.x - control.x) + pow(t1, 2.0) * (pos2.x - control.x);

		if (x1 >= fPos.x)
		{
			float dy1 = 2.0 * (t1 * aY + bY);
			
			int cross1 = int(sign(dy1));

			if ((!equalEpsilon(t1, 0.0, 0.0, epsilon) || cross1 != lastCrossDir) && (!lastContourSeg || !equalEpsilon(t1, 1.0, epsilon, 0.0) || cross1 != firstCrossDir))
			{
				result += cross1;
				lastCrossDir = cross1;

				if (firstCrossDir == 0 && equalEpsilon(t1, 0.0, 0.0, epsilon))
					firstCrossDir = cross1;
			}
		}
	}

	return result;
	#endif
}

void main()
{
	color = vec4(0.0, 0.0, 0.0, 1.0);

	fPosInt = ivec2(fPos);

	uint debugBegin = 0;
	uint debugEnd = -1;
	uint debugCount = 0;

	int windingNum = 0;

	bool newContour = true;
	bool skipPoint = false;
	bool isBezier = false;
	ivec2 prevOnCurvePos;
	ivec2 prevOffCurvePos;
	uint contourBeginIndex;
	for (uint i = 0; i < numPoints && debugCount != debugEnd; ++i)
	{
		if (i == debugBegin)
		{
			windingNum = 0;
			color = vec4(0.0, 0.0, 0.0, 1.0);
		}

		FontPoint currentPoint = fontPoints[symbolBegin + i];
		ivec2 pointPos = unpackPos(currentPoint.packedPos);

		bool onCurve = (currentPoint.flags & 0x1) == 0x1;

		if (newContour)
		{
			firstCrossDir = -2;
			lastCrossDir = 0;
			newContour = false;
			isBezier = false;

			contourBeginIndex = symbolBegin + i;
			prevOnCurvePos = pointPos;

			skipPoint = !onCurve;

			continue;
		}

		if (onCurve)
		{
			if (skipPoint)
				skipPoint = false;
			else
			{
				if (isBezier)
					windingNum += checkBezier(prevOnCurvePos, pointPos, prevOffCurvePos, false);
				else
					windingNum += checkLine(prevOnCurvePos, pointPos, false);

				++debugCount;

				if (firstCrossDir == -2)
				{
					firstCrossDir = 0;
				}
				
				if (firstCrossDir == 2)
				{
					firstCrossDir = -2;
				}
			}

			prevOnCurvePos = pointPos;
		}
		else
		{
			prevOffCurvePos = pointPos;
		}

		if (debugCount == debugEnd)
			break;

		isBezier = !onCurve;
		
		bool contourEnd = (currentPoint.flags & 0x2) == 0x2;

		if (contourEnd)
		{
			newContour = true;

			FontPoint contourBeginPoint = fontPoints[contourBeginIndex];
			ivec2 contourBeginPos = unpackPos(contourBeginPoint.packedPos);

			// Check if the first point of the contour is on the curve
			if ((contourBeginPoint.flags & 0x1) == 0x1)
			{
				if (isBezier)
					windingNum += checkBezier(prevOnCurvePos, contourBeginPos, pointPos, true);
				else
					windingNum += checkLine(pointPos, contourBeginPos, true);
			}
			else
			{
				FontPoint nextPoint = fontPoints[contourBeginIndex + 1];
				ivec2 nextPos = unpackPos(nextPoint.packedPos);

				windingNum += checkBezier(pointPos, nextPos, contourBeginPos, true);
			}

			++debugCount;
		}
	}

	//color = vec4(-firstCrossDir / 2.0, firstCrossDir / 2.0, 0.0, 1.0);
	//return;

	if (windingNum == 0)
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
		//color = vec4(1.0);
		color = vec4(-firstCrossDir / 2.0, firstCrossDir / 2.0, 1.0, 1.0);
	}
	#else
	color = vec4(1.0);
	#endif
}
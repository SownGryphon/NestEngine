#version 330 core

struct Shape
{
	vec3 origin;
	vec3 properties;
	vec3 color;
	float reflectiveness;
	/*	0 - Sphere
	 *	1 - Plane
	 *	2 - Box
	 */
	int shapeType;
};

Shape createSphere(vec3 origin, float size, vec3 color, float reflectiveness)
{
	return Shape(origin, vec3(size, 0., 0.), color, reflectiveness, 0);
}

Shape createPlane(vec3 origin, vec3 normal, vec3 color, float reflectiveness)
{
	return Shape(origin, normal, color, reflectiveness, 1);
}

Shape createBox(vec3 origin, vec3 size, vec3 color, float reflectiveness)
{
	return Shape(origin, size, color, reflectiveness, 2);
}

struct Light
{
	vec3 pos;
	float intensity;
	float k;
};

const float epsilon = 0.002, maxDist = 100;
const vec3 skyCol = vec3(0.65, 0.9, 1.);
const float ambientLight = 0.4;
const float diffuseCoeff = 3.;
const int maxBounces = 3;

uniform float u_aspect = 1.;
uniform float u_time = 0.;
uniform vec3 u_cameraPos = vec3(0.);
uniform mat3 u_cameraRot = mat3(1.);

const int numShapes = 8;
Shape shapes[numShapes] = Shape[](
	createSphere(vec3(0., 10., 2.), 2., vec3(0.15, 0.9, 0.9), 0.),
	createSphere(vec3(2., 11., 4.), 3., vec3(0.8, 0.8, 0.2), 0.1),
	createSphere(vec3(-3., 13., 3.5), 2.5, vec3(0.9, 0.9, 0.85), 0.),
	createSphere(vec3(-7., 6., 4.), 2.5, vec3(0.95, 0.95, 0.95), 0.9),
	createSphere(vec3(-7., -1., 4.), 2.5, vec3(0.95, 0.95, 0.95), 0.9),
	createSphere(vec3(-1. + sin(u_time / 1800) * 7., 12. + cos(u_time / 1800) * 7., 12.), 4, vec3(0.3, 0.3, 0.3), 0.85),
	createBox(vec3(4., 6., 1.), vec3(1.5, 3., 2.), vec3(0.1, 0.5, 0.2), 0.),
	createPlane(vec3(0., 0., 0.), vec3(0., 0., 1.), vec3(0.6), 0.)
);
const int numLights = 3;
Light lights[numLights] = Light[](
	Light(vec3(15., -20., 50.), 950., 0.1),
	Light(vec3(-30., 40., 35.), 1400., 0.12),
	Light(vec3(-50., 20., 10.), 900., 0.7)
);

in vec2 pos;

float sphereSDF(vec3 pos, Shape sphere)
{
	return length(pos - sphere.origin) - sphere.properties.x;
}

float planeSDF(vec3 pos, Shape plane)
{
	vec3 offset = pos - plane.origin;
	return dot(offset, plane.properties);
}

float boxSDF(vec3 pos, Shape box)
{
	vec3 q = abs(pos - box.origin) - (box.properties / 2);
	return length(max(q, 0.)) + min(max(q.x, max(q.y, q.z)), 0.);
}

float shapeSDF(vec3 pos, Shape shape)
{
	switch (shape.shapeType)
	{
	case 0:
		return sphereSDF(pos, shape);
	case 1:
		return planeSDF(pos, shape);
	case 2:
		return boxSDF(pos, shape);
	}
	return maxDist;
}

struct SphereTraceHit
{
	float dist;
	vec3 color;
	float reflectiveness;
};

SphereTraceHit sceneSDF(vec3 pos)
{
	SphereTraceHit hit = SphereTraceHit(maxDist, vec3(0.), 0.);
	for (int i = 0; i < numShapes; i++)
	{
		float shapeDist = shapeSDF(pos, shapes[i]);
		float diffStep = step(0., hit.dist - shapeDist);

		hit.dist = mix(hit.dist, shapeDist, diffStep);
		hit.color = mix(hit.color, shapes[i].color, diffStep);
		hit.reflectiveness = mix(hit.reflectiveness, shapes[i].reflectiveness, diffStep);
	}
	return hit;
}

vec3 estimateNormal(vec3 pos)
{
	float x = sceneSDF(vec3(pos.x + epsilon, pos.y, pos.z)).dist - sceneSDF(vec3(pos.x - epsilon, pos.y, pos.z)).dist;
    float y = sceneSDF(vec3(pos.x, pos.y + epsilon, pos.z)).dist - sceneSDF(vec3(pos.x, pos.y - epsilon, pos.z)).dist;
    float z = sceneSDF(vec3(pos.x, pos.y, pos.z + epsilon)).dist - sceneSDF(vec3(pos.x, pos.y, pos.z - epsilon)).dist;
    return normalize(vec3(x,y,z));
}

float computeShadow(vec3 pos)
{
	float aggregateLight = ambientLight;
	vec3 normal = estimateNormal(pos);
	vec3 startPos = pos + normal * epsilon;
	for (int i = 0; i < numLights; i++)
	{
		vec3 lightDir = normalize(lights[i].pos - startPos);
		if (dot(lightDir, normal) < 0)
			continue;

		float t = epsilon * 2;
		float distToLight = length(lights[i].pos - startPos);
		float res = 1.;
		float pDist = 1e20;
		while (t < distToLight)
		{
			vec3 scanPos = startPos + lightDir * t;
			SphereTraceHit hit = sceneSDF(scanPos);
			if (hit.dist < epsilon)
			{
				res = 0.;
				break;
			}

			float y = hit.dist * hit.dist / (2. * pDist);
			float d = sqrt(hit.dist * hit.dist - y * y);
			res = min(res, d / (lights[i].k * max(0., t - y)));
			t += hit.dist;
			pDist = hit.dist;
		}

		float localIntensity = lights[i].intensity / (distToLight * distToLight);
		float lightMult = min(min(diffuseCoeff * dot(lightDir, normal), 1.), res);
		aggregateLight += localIntensity * lightMult;
	}
	return aggregateLight;
}

void main()
{
	vec2 uv = pos * 2. - 1.;
	uv.x *= u_aspect;

	vec3 scanPos = u_cameraPos;
	vec3 dir = u_cameraRot * normalize(vec3(uv.x, 1.3, uv.y));
	vec3 color = skyCol;
	float reflectionMult = 1.;

	for (int i = 0; i <= maxBounces && reflectionMult != 0.; i++)
	{
		float dist = 0.;
		while (dist < maxDist)
		{
			SphereTraceHit hit = sceneSDF(scanPos);
			if (hit.dist < epsilon)
			{
				float shadow = computeShadow(scanPos);
				vec3 pointColor = hit.color * shadow;
				color = mix(color, pointColor, reflectionMult);
				reflectionMult *= hit.reflectiveness;
				break;
			}

			scanPos += dir * hit.dist;
			dist += hit.dist;
		}

		if (dist >= maxDist)
		{
			color = mix(color, skyCol, reflectionMult);
			break;
		}

		// Reflect the ray
		vec3 normal = estimateNormal(scanPos);
		vec3 dirNormComp = dot(normal, dir) * normal;
		dir -= dirNormComp * 2.;
		scanPos += dir * epsilon * 2.;
	}
	gl_FragColor = vec4(color, 1.);
}
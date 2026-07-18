#pragma once
#include "engine.h"
#include "settings.h"
#include <float.h>

//#define GetDistance3D(a,b) (sqrtf(((a)[0]-(b)[0])*((a)[0]-(b)[0])+((a)[1]-(b)[1])*((a)[1]-(b)[1])+((a)[2]-(b)[2])*((a)[2]-(b)[2])))

static float GetDistance3D(float* a, float* b)
{
	float dx = a[0] - b[0];
	float dy = a[1] - b[1];
	float dz = a[2] - b[2];
	return sqrtf(dx * dx + dy * dy + dz * dz);
}

// Direction vector
static float* GetDistance(vec3_t target, vec3_t player)
{
	vec3_t result;
	result[0] = target[0] - player[0];
	result[1] = target[1] - player[1];
	result[2] = target[2] - player[2];

	return result;
}

namespace features
{
	void standard_aim();
	void silent_aim();
}


#include "Random.h"

#include <random>

namespace Nest
{
	void Random::init(unsigned int seed)
	{
		std::srand(seed);
	}

	float Random::random(float min, float max)
	{
		float offset = float(std::rand()) / RAND_MAX;
		return (max - min) * offset + min;
	}

	int Random::randomInt(int min, int max)
	{
		return std::rand() % (max - min) + min;
	}

	float Random::randomGeometric(float base)
	{
		float power = random(-1, 1);
		return std::pow(base, power);
	}
}
#pragma once

namespace Nest
{
	namespace Random
	{
		void init(unsigned int seed);

		// Returns a random number between min and max
		float random(float min, float max);
		// Returns a random integer in the range [min, max - 1]
		int randomInt(int min, int max);
		// Returns a random number between (base)^-1 and base
		// The average returned value is 1
		float randomGeometric(float base);
	}
}
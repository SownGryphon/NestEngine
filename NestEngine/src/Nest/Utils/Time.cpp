#include "Time.h"

using namespace std::chrono;

milliseconds Nest::Time::getTimeMillis()
{
	return duration_cast<milliseconds>(steady_clock::now().time_since_epoch());
}
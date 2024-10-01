#include "MovingAverage.h"

MovingAverage::MovingAverage(size_t numPoints) :
	m_window(numPoints, 0.f)
{}

void MovingAverage::add(float val)
{
	m_window[m_lastIndex] = val;
	m_lastIndex = (m_lastIndex + 1) % m_window.size();
}

float MovingAverage::get()
{
	float res = 0.f;
	for (float f : m_window)
		res += f;
	return res / m_window.size();
}

void MovingAverage::setSize(size_t newSize)
{
	m_window.resize(newSize);
	m_lastIndex %= newSize;
}
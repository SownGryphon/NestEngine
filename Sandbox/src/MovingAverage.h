#pragma once

#include <vector>

class MovingAverage
{
public:

	MovingAverage(size_t numPoints);

	void add(float val);
	float get();

	void setSize(size_t newSize);
	inline size_t getSize() const { return m_window.size(); }

private:

	std::vector<float> m_window;
	size_t m_lastIndex = 0;
};
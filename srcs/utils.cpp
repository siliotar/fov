#include "utils.hpp"

float	getSqDistance(const Point &p1, const Point &p2)
{
	return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

float	getVecLength(const Point &point)
{
	return sqrtf(point.x * point.x + point.y * point.y);
}

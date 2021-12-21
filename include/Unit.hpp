#pragma once

#include "Point.hpp"
#include "utils.hpp"

struct Unit
{
	Unit() {}
	Unit(float x1, float y1, float x2, float y2) : pos(x1, y1), direction(x2, y2)
	{
		dirLength = getVecLength(direction);
	}
	Point	pos;
	Point	direction;
	float	dirLength;
};

inline bool operator==(const Unit& lhs, const Unit& rhs)
{
	return lhs.pos == rhs.pos;
}

inline bool operator<(const Unit& lhs, const Unit& rhs)
{
	return lhs.pos < rhs.pos;
}

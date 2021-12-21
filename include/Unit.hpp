#pragma once

#include "Point.hpp"

struct Unit
{
	Unit() {}
	Unit(float x1, float y1, float x2, float y2) : pos(x1, y1), direction(x2, y2) {}
	Point	pos;
	Point	direction;
};

#pragma once

struct Point
{
	Point(): x(0.0f), y(0.0f) {}
	Point(float x, float y) : x(x), y(y) {}
	Point(const Point &copy): x(copy.x), y(copy.y) {}
	const Point	&operator=(const Point &other)
	{
		if (this != &other)
		{
			this->x = other.x;
			this->y = other.y;
		}
		return *this;
	}
	float	x;
	float	y;
};

inline bool operator==(const Point& lhs, const Point& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

inline bool operator<(const Point& lhs, const Point& rhs)
{
	return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y);
}

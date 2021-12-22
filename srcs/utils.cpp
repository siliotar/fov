#include "utils.hpp"

float	getSqDistance(const Point &p1, const Point &p2)
{
	return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

float	getVecLength(const Point &point)
{
	return sqrtf(point.x * point.x + point.y * point.y);
}

std::string		readFile(const std::string &filePath)
{
	std::string		line;
	std::ifstream	f(filePath.c_str());
	std::stringstream	ss;
	if (f.is_open())
	{
		while (getline(f, line))
			ss << line << "\n";
		f.close();
	}
	return ss.str();
}

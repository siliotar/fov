#include "Config.hpp"

float	getDistance(const Point &p1, const Point &p2)
{
	float	dx = p1.x - p2.x;
	float	dy = p1.y - p2.y;
	return dx * dx + dy * dy;
}

float	getVecLength(const Point &point)
{
	return sqrtf(point.x * point.x + point.y * point.y);
}

size_t	calculateSee(const Config &conf, const Unit &unit)
{
	size_t	count = 0;
	float	distsq = conf.getDistance() * conf.getDistance();
	for (size_t i = 0; i < conf.getUnits().size(); ++i)
	{
		if (conf.getUnits()[i].pos == unit.pos)
			continue;
		if (getDistance(unit.pos, conf.getUnits()[i].pos) <= distsq)
		{
			Point	vec(conf.getUnits()[i].pos.x - unit.pos.x, conf.getUnits()[i].pos.y - unit.pos.y);
			float	scalar = unit.direction.x * vec.x + unit.direction.y * vec.y;
			float	modul = getVecLength(unit.direction) * getVecLength(vec);
			float	cosinus = scalar / modul;
			float	angle = acos(cosinus);
			if (angle <= conf.getFov() / 2.0f)
				++count;
		}
	}
	return count;
}

void	calculateResult(const Config &conf)
{
	std::vector<size_t>	result;
	result.reserve(conf.getUnits().size());
	for (size_t i = 0; i < conf.getUnits().size(); ++i)
	{
		result.push_back(calculateSee(conf, conf.getUnits()[i]));
	}
	for (size_t i = 0; i < result.size(); ++i)
	{
		std::cout << "Unit" << i + 1 << ": see " << result[i] << std::endl;
	}
}

int main(int argc, const char **argv)
{
	if (argc < 2)
	{
		std::cout << "Not enough arguments!" << std::endl;
		return EXIT_FAILURE;
	}
	try
	{
		Config	conf(argv[1]);
		calculateResult(conf);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	// for (size_t i = 0; i < 15000; ++i)
	// 	std::cout << "pos (" << i << ", -1) dir (0.707, 0.707)" << std::endl;
}

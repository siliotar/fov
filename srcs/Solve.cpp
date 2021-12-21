#include "Solve.hpp"

size_t	calculateSee(const Config &conf, const Unit &unit, size_t unitPos)
{
	size_t	count = 0;
	float	dist = conf.getDistance();
	float	distsq = conf.getDistance() * conf.getDistance();
	const std::vector<Unit> &units = conf.getUnits();
	size_t	size = units.size();
	const Unit	*data = units.data();
	for (size_t i = 0; i < size; ++i)
	{
		if (i == unitPos)
			continue;
		const Unit	&tUnit = *(data + i);
		float	dx = tUnit.pos.x - unit.pos.x;
		if (dx > dist)
			continue;
		float	dy = tUnit.pos.y - unit.pos.y;
		if (dy > dist)
			continue;
		if (dx * dx + dy * dy <= distsq)
		{
			Point	vec(tUnit.pos.x - unit.pos.x, tUnit.pos.y - unit.pos.y);
			float	scalar = unit.direction.x * vec.x + unit.direction.y * vec.y;
			float	modul = unit.dirLength * getVecLength(vec);
			float	cosinus = scalar / modul;
			float	angle = acos(cosinus);
			if (angle <= conf.getFov() / 2.0f)
				++count;
		}
	}
	return count;
}

void	calculateResultThread(const Config &conf, size_t i, size_t *result)
{
	const std::vector<Unit> &units = conf.getUnits();
	size_t	start = i * ((float)units.size() / THREAD_COUNT);
	size_t	max = (i + 1) * ((float)units.size() / THREAD_COUNT);
	if (max > units.size())
		max = units.size();
	for (size_t j = start; j < max; ++j)
		result[j] = calculateSee(conf, units[j], j);
}

void	calculateResult(const Config &conf)
{
	std::vector<size_t>	result;
	size_t	unitsSize = conf.getUnits().size();
	result.reserve(unitsSize);
	std::thread	workers[THREAD_COUNT];
	for (int i = 0; i < THREAD_COUNT; ++i)
		workers[i] = std::thread(calculateResultThread, conf, i, result.data());
	for (int i = 0; i < THREAD_COUNT; ++i)
		workers[i].join();
	for (size_t i = 0; i < unitsSize; ++i)
		std::cout << "Unit" << i + 1 << ": see " << result[i] << "\n";
}

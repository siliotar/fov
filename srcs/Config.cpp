#include "Config.hpp"

Config::Config(const std::string &filePath) : _fov(-1.0f), _dist(-1.0f)
{
	std::ifstream	confFile(filePath);
	std::stringstream	ss;
	std::string	line;

	if (!confFile.is_open())
		throw WrongFileException();
	size_t	lineCount = 0;
	while (std::getline(confFile, line))
		if (line.size() != 0)
			++lineCount;
	_units.reserve(lineCount);
	confFile.clear();
	confFile.seekg(0, std::ios::beg);

	while (std::getline(confFile, line))
	{
		if (line.size() == 0)
			continue ;
		if (line.find("fov") != std::string::npos)
			_fov = _parseValue(line, "fov");
		else if (line.find("distance") != std::string::npos)
			_dist = _parseValue(line, "distance");
		else
			_parsePoints(line);
	}
	if (_fov <= 0.0f || _dist <= 0)
		throw IncorrectFileException();
	if (_fov > 360.0f)
		_fov = M_PI * 2;
	else
		_fov = _fov / 180.0f * M_PI;
	_units.resize(_units.size());
	std::vector<Point>	posVector;
	posVector.reserve(_units.size());
	for (size_t i = 0; i < _units.size(); ++i)
		posVector.emplace_back(_units[i].pos);
	std::sort(posVector.begin(), posVector.end());
	std::vector<Point>::const_iterator	it = posVector.begin() + 1;
	std::vector<Point>::const_iterator	prev = posVector.begin();
	for (; it != posVector.end(); ++it)
	{
		if (*it == *prev)
			throw SameCoordinatesException();
		prev = it;
	}
}

float	Config::_parseValue(const std::string &line, const std::string &name)
{
	std::istringstream	ssline(line);
	std::string	temp;
	std::string	temp2;
	float	value;
	ssline >> temp >> value >> temp2;
	if (temp != name || value <= 0.0f || temp2.size() > 0 || !ssline.eof())
		throw IncorrectFileException();
	return value;
}

void	Config::_parsePoints(const std::string &line)
{
	std::istringstream	ssline(line);
	std::string	pos;
	std::string	dir, temp;
	char	openBracket1, comma1, closeBracket1, openBracket2, comma2, closeBracket2;
	float	xPos, yPos, xDir, yDir;
	ssline >> pos >> openBracket1 >> xPos >> comma1 >> yPos >> closeBracket1;
	ssline >> dir >> openBracket2 >> xDir >> comma2 >> yDir >> closeBracket2;
	ssline >> temp;
	if (pos != "pos" || openBracket1 != '(' || comma1 != ',' || closeBracket1 != ')' \
		|| dir != "dir" || openBracket2 != '(' || comma2 != ',' || closeBracket2 != ')' \
		|| temp.size() > 0 || !ssline.eof())
		throw IncorrectFileException();
	_units.emplace_back(xPos, yPos, xDir, yDir);
}

Config::~Config() {}

float	Config::getFov() const
{
	return _fov;
}

float	Config::getDistance() const
{
	return _dist;
}

const std::vector<Unit>	&Config::getUnits() const
{
	return _units;
}


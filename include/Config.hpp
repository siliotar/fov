#pragma once

#include <vector>
#include <exception>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include "Unit.hpp"

class Config
{
private:
	float	_fov;
	float	_dist;
	std::vector<Unit>	_units;
	Config();
	float	_parseValue(const std::string &line, const std::string &name);
	void	_parsePoints(const std::string &line);
public:
	Config(const std::string &filePath);
	~Config();
	class SameCoordinatesException : public std::exception
	{
	public:
		SameCoordinatesException() {}
		virtual ~SameCoordinatesException() {}
		virtual const char* what() const throw () { return "Two units have the same coorinates"; }
	};
	class WrongFileException : public std::exception
	{
	public:
		WrongFileException() {}
		virtual ~WrongFileException() {}
		virtual const char* what() const throw () { return "Wrong config file"; }
	};
	class IncorrectFileException : public std::exception
	{
	public:
		IncorrectFileException() {}
		virtual ~IncorrectFileException() {}
		virtual const char* what() const throw () { return "Incorrect config file"; }
	};
	float	getFov() const;
	float	getDistance() const;
	const std::vector<Unit>	&getUnits() const;
};

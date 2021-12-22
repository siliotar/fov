#pragma once

#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include "Point.hpp"

float	getSqDistance(const Point &p1, const Point &p2);
float	getVecLength(const Point &point);
std::string		readFile(const std::string &filePath);

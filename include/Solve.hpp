#pragma once

#include <thread>
#include <future>
#include <iostream>
#include "Config.hpp"
#include "utils.hpp"

#define THREAD_COUNT 8

void	calculateResult(const Config &conf);

#include "Config.hpp"
#include "Solve.hpp"
#include <iostream>
#include <chrono>

int main(int argc, const char **argv)
{
	using namespace std::literals::chrono_literals;
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

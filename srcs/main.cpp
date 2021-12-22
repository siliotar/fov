#include "Config.hpp"
#include "Solve.hpp"
#include <iostream>
#include "Visualizer.hpp"

int	returnErorr(const std::string &msg)
{
	std::cout << msg << std::endl;
	std::cout << "Usage: fov [-v] configuration_file\n";
	std::cout << "-v\tVisualize\n";
	return EXIT_FAILURE;
}

int main(int argc, const char **argv)
{
	bool	visualize = false;
	if (argc < 2)
		return returnErorr("Not enough arguments!");
	else if (argc == 3 && std::string(argv[1]) == "-v")
		visualize = true;
	else if (argc >= 3)
		return returnErorr("Too many arguments!");
	try
	{
		Config	conf(argv[1 + visualize]);
		calculateResult(conf);
		if (visualize)
		{
			Visualizer	v(conf);
			v.visualize();
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}

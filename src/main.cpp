/*
Created by: Emily (Em_iIy) Winnink
Created on: 06/09/2024
*/
#include "Scop.hpp"
#include "Manager.hpp"
#include "Config.hpp"

#define WIDTH 1920
#define HEIGHT 1080

float		g_delta_time = 0.0f;
Scop		scop;

static void	clean(void)
{
	Manager::clear();
	scop.del();
}

int	main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Invalid input!\n";
		std::cerr << "Usage:\n";
		std::cerr << "scop -c <config file> (WIP)\n";
		std::cerr << "scop <object file> <texture file>" << std::endl;
		return (1);
	}
	try
	{
		scop.init_gl(WIDTH, HEIGHT);
		if (std::string(argv[1]) == "-c")
		{
			Config::load(argv[2]);
			Config::print();
		}
		else
		{
			scop.init_resources(argv[1], argv[2]);
		}
		print_controls();
		while (!glfwWindowShouldClose(scop.window))
		{
			scop.update();
			scop.draw_current();
		}
		clean();
	}
	catch(const std::exception& e)
	{
		clean();
		return (1);
	}
	return (0);
}

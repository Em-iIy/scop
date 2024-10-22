/*
Created by: Emily (Em_iIy) Winnink
Created on: 06/09/2024
*/
#include <iostream>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <emlm/emlm.hpp>
#include <emlm/mlm/print/vectorPrint.hpp>

#include "Scop.hpp"
#include "Camera.hpp"
#include "utils.hpp"
#include "input.hpp"
#include "Object.hpp"
#include "Manager.hpp"

#define WIDTH 1920
#define HEIGHT 1080

float		g_delta_time = 0.0f;
Scop		scop;

int	main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Missing input files!" << std::endl;
		return (1);
	}
	try
	{
	scop.init_gl(WIDTH, HEIGHT);
	scop.init_resources(argv[1], argv[2]);

	print_controls();
	while (!glfwWindowShouldClose(scop.window))
	{
		scop.update();

		scop.draw_current();
	
	}
	scop.del();
	Manager::clear();
	// glfwTerminate(); // Can cause leaks occasionally
	}
	catch(const std::exception& e)
	{
		// std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}
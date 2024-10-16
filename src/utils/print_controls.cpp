#include <iostream>

void	print_controls(void)
{
	std::cout << "up/down arrow:\t\tMove object on z axis" << std::endl;
	std::cout << "left/right arrow:\tMove object on x axis" << std::endl;
	std::cout << "Right shift/Space:\tMove object on y axis" << std::endl;
	std::cout << "-/+:\t\t\tScale object up and down" << std::endl;
	std::cout << "W/S:\t\t\tMove camera on z axis" << std::endl;
	std::cout << "A/D:\t\t\tMove camera on x axis" << std::endl;
	std::cout << "Left shift/Space:\tMove camera on y axis" << std::endl;
	std::cout << "Mouse:\t\t\tLook around" << std::endl;
	std::cout << "esc:\t\t\tQuit program" << std::endl;
	std::cout << "Tab:\t\t\tEnable texture mode" << std::endl;
	std::cout << "1:\t\t\tEnable wireframe mode" << std::endl;
}

#pragma once

#include <GLFW/glfw3.h>

void	process_input(GLFWwindow *window);
void	mouse_callback(GLFWwindow *window, double xposIn, double yposIn);
void	framebuffer_size_callback(GLFWwindow *window, int width, int height);

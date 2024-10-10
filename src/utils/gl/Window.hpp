/*
Created by: Emily (Em_iIy) Winnink
Created on: 07/09/2024
*/
#pragma once

#include <GLFW/glfw3.h>
#include <GLAD/glad.h>

void	init_glfw(void);
GLFWwindow	*init_window(int width, int height, const char *title, GLFWmonitor *monitor, GLFWwindow *share);

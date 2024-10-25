#pragma once

#include <string>

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.hpp"
#include "input.hpp"
#include "utils.hpp"

struct Initted
{
	bool	gl;
	bool	window;
	bool	resources;
};


struct State
{
	int		width;
	int		height;
	int		tex_mode;
	int		wf_mode;
	float	angle;
	float	tex_mix;
	float	rotate_speed;
};


class Scop {
	private:
		
	public:
		Camera		camera;
		State		state;
		Initted		initted;
		GLFWwindow	*window;

		std::string	current_object;
		std::string	current_texture;
		std::string	current_shader;

		VAO			vao;
		VBO			vbo;
		EBO			ebo;

		Scop();

		void	del();

		void	init_gl(const int w, const int h);

		void	init_resources(const char *object, const char *texture);

		void	update();
		void	draw_current();
};

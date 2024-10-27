#pragma once

#include <string>

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.hpp"
#include "Object.hpp"
#include "input.hpp"
#include "utils.hpp"

struct Initted
{
	bool	gl;
	bool	window;
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

		uint	current_object;
		std::vector<std::string> object_names;
		uint	current_texture;
		std::vector<std::string> texture_names;
		uint	current_shader;
		std::vector<std::string> shader_names;

		Scop();

		void	del();

		void	init_gl(const int w, const int h);

		void	init_resources();

		void	update();
		void	draw_current();

		void	change_object(const int direction);
		void	change_texture(const int direction);
		void	change_shader(const int direction);

		Object	&get_current_object();
		Tex2d	&get_current_texture();
		Shader	&get_current_shader();
};

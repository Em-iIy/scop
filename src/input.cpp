/*
Created by: Emily (Em_iIy) Winnink
Created on: 09/10/2024
*/
#include "utils/utils.hpp"
#include <GLFW/glfw3.h>

#include "Camera.hpp"
#include "Object.hpp"
#include "Manager.hpp"

#define WIDTH 1920
#define HEIGHT 1080


extern Camera		camera;
extern std::string	current_object;

extern int			tex_mode;
extern float		g_delta_time;

extern int			g_width;
extern int			g_height;

static void	render_options(GLFWwindow *window)
{
	static int	wf_mode = 0;

	static Key esc(window, GLFW_KEY_ESCAPE);
	static Key one(window, GLFW_KEY_1);
	static Key tab(window, GLFW_KEY_TAB);

	one.update();
	tab.update();
	esc.update();

	if (one.is_pressed())
	{
		++wf_mode;
		if (wf_mode % 2 == 0)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (tab.is_pressed())
	{
		++tex_mode;
	}
	if (esc.is_pressed())
	{
		glfwSetWindowShouldClose(window, true);
	}
}

static void	camera_movement(GLFWwindow *window)
{
	static Key w(window, GLFW_KEY_W);
	static Key a(window, GLFW_KEY_A);
	static Key s(window, GLFW_KEY_S);
	static Key d(window, GLFW_KEY_D);
	static Key lshift(window, GLFW_KEY_LEFT_SHIFT);
	static Key space(window, GLFW_KEY_SPACE);

	w.update();
	a.update();
	s.update();
	d.update();
	lshift.update();
	space.update();

	if (w.is_down())
	{
		camera.process_keyboard(FORWARD, g_delta_time);
	}
	if (a.is_down())
	{
		camera.process_keyboard(LEFT, g_delta_time);
	}
	if (s.is_down())
	{
		camera.process_keyboard(BACKWARD, g_delta_time);
	}
	if (d.is_down())
	{
		camera.process_keyboard(RIGHT, g_delta_time);
	}
	if (lshift.is_down())
	{
		camera.process_keyboard(DOWN, g_delta_time);
	}
	if (space.is_down())
	{
		camera.process_keyboard(UP, g_delta_time);
	}
}

static void	object_movement(GLFWwindow *window)
{
	static Key up(window, GLFW_KEY_UP);
	static Key down(window, GLFW_KEY_DOWN);
	static Key left(window, GLFW_KEY_LEFT);
	static Key right(window, GLFW_KEY_RIGHT);
	static Key rshift(window, GLFW_KEY_RIGHT_SHIFT);
	static Key rctrl(window, GLFW_KEY_RIGHT_CONTROL);
	static Key plus(window, GLFW_KEY_EQUAL); // for the button with the + on it
	static Key minus(window, GLFW_KEY_MINUS);

	Object	&obj = Manager::get_object(current_object);

	up.update();
	down.update();
	left.update();
	right.update();
	rshift.update();
	rctrl.update();
	plus.update();
	minus.update();

	if (up.is_down())
	{
		obj.process_keyboard(OBJ_FORWARD, g_delta_time);
	}
	if (left.is_down())
	{
		obj.process_keyboard(OBJ_LEFT, g_delta_time);
	}
	if (down.is_down())
	{
		obj.process_keyboard(OBJ_BACKWARD, g_delta_time);
	}
	if (right.is_down())
	{
		obj.process_keyboard(OBJ_RIGHT, g_delta_time);
	}
	if (rctrl.is_down())
	{
		obj.process_keyboard(OBJ_DOWN, g_delta_time);
	}
	if (rshift.is_down())
	{
		obj.process_keyboard(OBJ_UP, g_delta_time);
	}
	if (plus.is_down())
	{
		obj.process_keyboard(OBJ_SCALE_UP, g_delta_time);
	}
	if (minus.is_down())
	{
		obj.process_keyboard(OBJ_SCALE_DOWN, g_delta_time);
	}
}

void	process_input(GLFWwindow *window)
{
	render_options(window);
	object_movement(window);
	camera_movement(window);
}

void	mouse_callback(GLFWwindow *window, double in_xpos, double in_ypos)
{
	static float	last_x = WIDTH / 2.0f;
	static float	last_y = HEIGHT / 2.0f;
	static bool		first_mouse = true;

	float xpos = static_cast<float>(in_xpos);
	float ypos = static_cast<float>(in_ypos);

	(void)window;
	if (first_mouse)
	{
		last_x = xpos;
		last_y = ypos;
		first_mouse = false;
	}
	float xoffset = xpos - last_x;
	float yoffset = last_y - ypos;

	last_x = xpos;
	last_y = ypos;

	camera.process_mouse_movement(xoffset, yoffset, true);
}

void	framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	(void)window;
	g_width = width;
	g_height = height;
	glViewport(0, 0, width, height);
}

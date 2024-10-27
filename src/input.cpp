/*
Created by: Emily (Em_iIy) Winnink
Created on: 09/10/2024
*/
#include "utils/utils.hpp"

#include "Manager.hpp"
#include "Scop.hpp"

extern Scop	scop;
extern float		g_delta_time;


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
		++scop.state.tex_mode;
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
		scop.camera.process_keyboard(FORWARD, g_delta_time);
	}
	if (a.is_down())
	{
		scop.camera.process_keyboard(LEFT, g_delta_time);
	}
	if (s.is_down())
	{
		scop.camera.process_keyboard(BACKWARD, g_delta_time);
	}
	if (d.is_down())
	{
		scop.camera.process_keyboard(RIGHT, g_delta_time);
	}
	if (lshift.is_down())
	{
		scop.camera.process_keyboard(DOWN, g_delta_time);
	}
	if (space.is_down())
	{
		scop.camera.process_keyboard(UP, g_delta_time);
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
	static Key plus(window, GLFW_KEY_EQUAL); // for the key with the + on it
	static Key minus(window, GLFW_KEY_MINUS);

	Object	&obj = scop.get_current_object();

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

static void	switch_renders(GLFWwindow *window)
{
	static Key	num7(window, GLFW_KEY_KP_7);
	static Key	num9(window, GLFW_KEY_KP_9);
	static Key	num4(window, GLFW_KEY_KP_4);
	static Key	num6(window, GLFW_KEY_KP_6);
	static Key	num1(window, GLFW_KEY_KP_1);
	static Key	num3(window, GLFW_KEY_KP_3);

	num7.update();
	num9.update();
	num4.update();
	num6.update();
	num1.update();
	num3.update();

	if (num7.is_pressed())
	{
		scop.change_object(-1);
	}
	if (num9.is_pressed())
	{
		scop.change_object(1);
	}
	if (num4.is_pressed())
	{
		scop.change_texture(-1);
	}
	if (num6.is_pressed())
	{
		scop.change_texture(1);
	}
	if (num1.is_pressed())
	{
		scop.change_shader(-1);
	}
	if (num3.is_pressed())
	{
		scop.change_shader(1);
	}
}

void	process_input(GLFWwindow *window)
{
	render_options(window);
	object_movement(window);
	camera_movement(window);
	switch_renders(window);
}

void	mouse_callback(GLFWwindow *window, double in_xpos, double in_ypos)
{
	static float	last_x = scop.state.width / 2.0f;
	static float	last_y = scop.state.height / 2.0f;
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

	scop.camera.process_mouse_movement(xoffset, yoffset, true);
}

void	framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	(void)window;
	scop.state.width = width;
	scop.state.height = height;
	glViewport(0, 0, width, height);
}

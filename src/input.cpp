/*
Created by: Emily (Em_iIy) Winnink
Created on: 09/10/2024
*/
#include "utils/utils.hpp"
#include <GLFW/glfw3.h>

#include "Camera.hpp"

// include defines from one place/accessible through config
#define MOVE_SPEED 10.0f
#define SCALE_SPEED 5.0f

#define WIDTH 1920
#define HEIGHT 1080

int				texMode = 1;
int				wfMode = 0;

extern Camera	camera;
extern float	g_delta_time;

extern mlm::vec3	obj_pos;
extern mlm::vec3	obj_scale;

static void	render_options(GLFWwindow *window)
{
	static Key esc(window, GLFW_KEY_ESCAPE);
	static Key one(window, GLFW_KEY_1);
	static Key tab(window, GLFW_KEY_TAB);

	one.update();
	tab.update();
	esc.update();

	if (one.is_pressed())
	{
		++wfMode;
		if (wfMode % 2 == 0)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (tab.is_pressed())
	{
		++texMode;
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
		camera.processKeyboard(FORWARD, g_delta_time);
	}
	if (a.is_down())
	{
		camera.processKeyboard(LEFT, g_delta_time);
	}
	if (s.is_down())
	{
		camera.processKeyboard(BACKWARD, g_delta_time);
	}
	if (d.is_down())
	{
		camera.processKeyboard(RIGHT, g_delta_time);
	}
	if (lshift.is_down())
	{
		camera.processKeyboard(DOWN, g_delta_time);
	}
	if (space.is_down())
	{
		camera.processKeyboard(UP, g_delta_time);
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
		obj_pos += mlm::vec3(0.0f, 0.0f, -1.0f) * g_delta_time * MOVE_SPEED;
	}
	if (left.is_down())
	{
		obj_pos += mlm::vec3(-1.0f, 0.0f, 0.0f) * g_delta_time * MOVE_SPEED;
	}
	if (down.is_down())
	{
		obj_pos += mlm::vec3(0.0f, 0.0f, 1.0f) * g_delta_time * MOVE_SPEED;
	}
	if (right.is_down())
	{
		obj_pos += mlm::vec3(1.0f, 0.0f, 0.0f) * g_delta_time * MOVE_SPEED;
	}
	if (rctrl.is_down())
	{
		obj_pos += mlm::vec3(0.0f, -1.0f, 0.0f) * g_delta_time * MOVE_SPEED;
	}
	if (rshift.is_down())
	{
		obj_pos += mlm::vec3(0.0f, 1.0f, 0.0f) * g_delta_time * MOVE_SPEED;
	}
	if (plus.is_down())
	{
		obj_scale += g_delta_time * SCALE_SPEED;
	}
	if (minus.is_down())
	{
		obj_scale -= g_delta_time * SCALE_SPEED;
	}
}

void	process_input(GLFWwindow *window)
{
	render_options(window);
	object_movement(window);
	camera_movement(window);
}

void	mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
	static float	lastY = HEIGHT / 2.0f;
	static float	lastX = WIDTH / 2.0f;
	static bool		firstMouse = true;

	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	(void)window;
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.processMouseMovement(xoffset, yoffset, true);
}
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

#include "Camera.hpp" 
#include "utils.hpp"
#include "Object/Object.hpp"

#define WIDTH 1920
#define HEIGHT 1080
#define FOV	45.0f
#define MOVE_SPEED 10.0f
#define SCALE_SPEED 5.0f

int			texMode = 1;
int			wfMode = 0;
float		g_delta_time = 0.0f;
mlm::vec3	obj_pos = mlm::vec3(0.0f, 0.0f, -10.0f);
mlm::vec3	obj_scale = mlm::vec3(1.0f);

// move elsewhere/make static
float lastY = HEIGHT / 2.0f;
float lastX = WIDTH / 2.0f;
bool firstMouse = true;

Camera camera;

float deltaTimeUpdate(void)
{
	static float last_frame = 0.0f;
	float current_frame = static_cast<float>(glfwGetTime());
	float delta_time = current_frame - last_frame;
	last_frame = current_frame;
	return (delta_time);
}


void	m4print(const mlm::mat4 &m)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
			std::cout << m[j][i] << " ";
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void	mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
	(void)window;
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

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

void	processInput(GLFWwindow *window)
{
	// Render options
	static Key esc(window, GLFW_KEY_ESCAPE);
	static Key one(window, GLFW_KEY_1);
	static Key tab(window, GLFW_KEY_TAB);

	// Camera movement
	static Key w(window, GLFW_KEY_W);
	static Key a(window, GLFW_KEY_A);
	static Key s(window, GLFW_KEY_S);
	static Key d(window, GLFW_KEY_D);
	static Key lshift(window, GLFW_KEY_LEFT_SHIFT);
	static Key space(window, GLFW_KEY_SPACE);

	// Object movement
	static Key up(window, GLFW_KEY_UP);
	static Key down(window, GLFW_KEY_DOWN);
	static Key left(window, GLFW_KEY_LEFT);
	static Key right(window, GLFW_KEY_RIGHT);
	static Key rshift(window, GLFW_KEY_RIGHT_SHIFT);
	static Key rctrl(window, GLFW_KEY_RIGHT_CONTROL);
	static Key plus(window, GLFW_KEY_EQUAL); // for the button with the + on it
	static Key minus(window, GLFW_KEY_MINUS);

	

	// Render options
	one.update();
	tab.update();
	esc.update();

	// Camera movement
	w.update();
	a.update();
	s.update();
	d.update();
	lshift.update();
	space.update();

	// Object movement
	up.update();
	down.update();
	left.update();
	right.update();
	rshift.update();
	rctrl.update();
	plus.update();
	minus.update();

	// Render options
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

	// Camera movement
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
	

	// Object movement
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

static void	print_controls()
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

int	main(int argc, char **argv)
{
	srand(time(NULL));
	if (argc != 3)
	{
		std::cerr << "Missing input files!" << std::endl;
		return (1);
	}
	try
	{
	initGlfw();

	float start_time = glfwGetTime();
	Object obj(argv[1]);
	const mlm::vec3 obj_center = obj.get_center();
	std::cout << obj_center << std::endl;
	std::vector<Vertex> vertices = obj.get_vertices();
	std::vector<GLuint> indices = obj.get_indices();
	std::cout << "indices size: " << indices.size() << std::endl;
	std::cout << "vertices size: " << vertices.size() << std::endl;


	GLFWwindow *window = initWindow(WIDTH, HEIGHT, "scop", NULL, NULL);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);

	Shader shader("./resources/shaders/default.vert", "./resources/shaders/default.frag");
	GLuint texture = load_texture(argv[2]);
	shader.use();
	shader.setInt("boberTex", 0);



	VAO mainVao;
	mainVao.Bind();

	VBO mainVbo((GLfloat *)vertices.data(), vertices.size() * sizeof(Vertex));
	VBO normalsVbo((GLfloat *)vertices.data(), vertices.size() * sizeof(Vertex));
	EBO mainEbo((GLuint *)indices.data(), indices.size() * sizeof(GLuint));

	mainVao.LinkAtr(mainVbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void *)0);
	mainVao.LinkAtr(mainVbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void *)(3 * sizeof(GLfloat)));
	mainVao.LinkAtr(mainVbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void *)(6 * sizeof(GLfloat)));
	mainVao.LinkAtr(mainVbo, 3, 2, GL_FLOAT, sizeof(Vertex), (void *)(9 * sizeof(GLfloat)));
	mainVao.Unbind();

	float angle = 0.0f;
	float texMix = 0.0f;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	float end_time = glfwGetTime();
	std::cout << "init time:" << end_time - start_time << std::endl;
	print_controls();
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		g_delta_time = deltaTimeUpdate();
		// glClearColor(v4.x, v4.y, v4.z, 1.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		if (texMode % 2 == 0 && texMix < 1.0f)
		{
			texMix += g_delta_time;
		}
		else if (texMode % 2 == 1 && texMix > 0.0f)
		{
			texMix -= g_delta_time;
		}
		texMix = fminf32(fmaxf32(texMix, 0.0f), 1.0f);



		mlm::mat4 view = camera.get_matrix();
		mlm::mat4 projection = mlm::perspective(mlm::radians(FOV), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		mlm::mat4 model(1.0f);
		model = mlm::translate(model, obj_pos);
		model = mlm::scale(model, obj_scale);

		model = mlm::rotate(model, mlm::radians(90.0f), mlm::vec3(0.0f, 1.0f, 0.0f));
		// model = mlm::rotate(model, mlm::radians(-30.0f), mlm::vec3(1.0f, 0.0f, 0.0f));
		model = mlm::rotate(model, mlm::radians(angle), mlm::vec3(0.0f, 1.0f, 0.0f));
		// m = mlm::scale(m, mlm::vec3(dimValue));
		// m = mlm::rotate(m, mlm::radians(angle), mlm::vec3(1.0f, 1.0f, 0.0f));
		angle += 45.0f * g_delta_time;
		shader.setFloat("texMix", texMix);
		shader.setMat4("model", model);
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		mainVao.Bind();
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	mainEbo.Delete();
	mainVbo.Delete();
	mainVao.Delete();
	shader.Delete();
	delete_texture(texture);
	// glfwTerminate(); // Can cause leaks occasionally
	}
	catch(const std::exception& e)
	{
		// std::cerr << e.what() << std::endl;
	}
	return (0);
}
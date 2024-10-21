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
#include "input.hpp"
#include "Object/Object.hpp"
#include "Manager.hpp"

#define WIDTH 1920
#define HEIGHT 1080
#define FOV	45.0f

int			tex_mode = 1;
float		g_delta_time = 0.0f;
int			g_width = WIDTH;
int			g_height = HEIGHT;

Camera camera;
std::string	current_object = "input";

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
	init_glfw();

	Manager::load_object("input", argv[1]);

	float start_time = glfwGetTime();
	Manager::get_object("input").set_position(mlm::vec3(0.0f, 0.0f, -10.0f));
	Manager::get_object("input").set_scale(mlm::vec3(1.0f));
	const std::vector<Vertex> &vertices = Manager::get_object("input").get_vertices();
	const std::vector<GLuint> &indices = Manager::get_object("input").get_indices();


	GLFWwindow *window = init_window(g_width, g_height, "scop", NULL, NULL);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	Manager::load_shader("default", "./resources/shaders/default.vert", "./resources/shaders/default.frag");
	Manager::load_texture("input", argv[2]);
	std::cout << Manager::get_texture("input").get_ID() << std::endl;
	Manager::get_shader("default").use();
	Manager::get_shader("default").set_int("main_tex", 0);

	VAO mainVao;
	mainVao.bind();

	VBO mainVbo((GLfloat *)vertices.data(), vertices.size() * sizeof(Vertex));
	EBO mainEbo((GLuint *)indices.data(), indices.size() * sizeof(GLuint));

	mainVao.link_attr(mainVbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void *)0);
	mainVao.link_attr(mainVbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void *)(3 * sizeof(GLfloat)));
	mainVao.link_attr(mainVbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void *)(6 * sizeof(GLfloat)));
	mainVao.link_attr(mainVbo, 3, 2, GL_FLOAT, sizeof(Vertex), (void *)(9 * sizeof(GLfloat)));
	mainVao.unbind();

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
		process_input(window);
		g_delta_time = delta_time_update();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		if (tex_mode % 2 == 0 && texMix < 1.0f)
		{
			texMix += 1.0f * g_delta_time;
		}
		else if (tex_mode % 2 == 1 && texMix > 0.0f)
		{
			texMix -= 1.0f * g_delta_time;
		}
		texMix = fminf32(fmaxf32(texMix, 0.0f), 1.0f);


		angle += 45.0f * g_delta_time; // 45 degrees per second

		mlm::mat4 view = camera.get_matrix();

		mlm::mat4 projection = mlm::perspective(mlm::radians(FOV), (float)g_width / (float)g_height, 0.1f, 100.0f);
	
		mlm::mat4 model(1.0f);
		model = mlm::translate(model, Manager::get_object("input").get_position());
		model = mlm::scale(model, Manager::get_object("input").get_scale());
		model = mlm::rotate(model, mlm::radians(90.0f), mlm::vec3(0.0f, 1.0f, 0.0f));
		model = mlm::rotate(model, mlm::radians(angle), mlm::vec3(0.0f, 1.0f, 0.0f));
	
		Manager::get_shader("default").use();
		glActiveTexture(GL_TEXTURE0);
		Manager::get_texture("input").bind();
		Manager::get_shader("default").set_float("texMix", texMix);
		Manager::get_shader("default").set_mat4("model", model);
		Manager::get_shader("default").set_mat4("projection", projection);
		Manager::get_shader("default").set_mat4("view", view);
		mainVao.bind();
	
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
	
		glfwPollEvents();
	}
	mainEbo.del();
	mainVbo.del();
	mainVao.del();
	Manager::clear();
	// glfwTerminate(); // Can cause leaks occasionally
	}
	catch(const std::exception& e)
	{
		// std::cerr << e.what() << std::endl;
	}
	return (0);
}
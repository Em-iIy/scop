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

#define WIDTH 1920
#define HEIGHT 1080
#define FOV	45.0f

int			tex_mode = 1;
float		g_delta_time = 0.0f;
int			g_width = WIDTH;
int			g_height = HEIGHT;

Camera camera;
Object obj;

float delta_time_update(void)
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
	init_glfw();

	float start_time = glfwGetTime();
	obj.load(argv[1]);
	const mlm::vec3 obj_center = obj.get_center();
	obj.set_position(mlm::vec3(0.0f, 0.0f, -10.0f));
	obj.set_scale(mlm::vec3(1.0f));
	std::vector<Vertex> vertices = obj.get_vertices();
	std::vector<GLuint> indices = obj.get_indices();


	GLFWwindow *window = init_window(g_width, g_height, "scop", NULL, NULL);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	Shader shader("./resources/shaders/default.vert", "./resources/shaders/default.frag");
	Tex2d	texture;
	texture.load(argv[2]);
	shader.use();
	shader.set_int("main_tex", 0);

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
		model = mlm::translate(model, obj.get_position());
		model = mlm::scale(model, obj.get_scale());
		model = mlm::rotate(model, mlm::radians(90.0f), mlm::vec3(0.0f, 1.0f, 0.0f));
		model = mlm::rotate(model, mlm::radians(angle), mlm::vec3(0.0f, 1.0f, 0.0f));
	
		shader.use();
		glActiveTexture(GL_TEXTURE0);
		texture.bind();
		shader.set_float("texMix", texMix);
		shader.set_mat4("model", model);
		shader.set_mat4("projection", projection);
		shader.set_mat4("view", view);
		mainVao.bind();
	
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
	
		glfwPollEvents();
	}
	mainEbo.del();
	mainVbo.del();
	mainVao.del();
	shader.del();
	texture.del();
	// glfwTerminate(); // Can cause leaks occasionally
	}
	catch(const std::exception& e)
	{
		// std::cerr << e.what() << std::endl;
	}
	return (0);
}
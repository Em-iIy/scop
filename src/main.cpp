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

#include "utils.hpp"
#include "Object/Object.hpp"

#define WIDTH 1920
#define HEIGHT 1080
#define FOV	60.0f

int texMode = 1;


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

void	processInput(GLFWwindow *window)
{
	static int wfMode = 0;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	
	static int key_tab_state = GLFW_RELEASE;
	if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE)
		key_tab_state = GLFW_RELEASE;
	else if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS && key_tab_state == GLFW_RELEASE)
	{
		key_tab_state = GLFW_PRESS;
		++wfMode;
		++texMode;
		// if (wfMode % 2 == 0)
		// 	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		// else
		// 	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
}

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Missing input files!" << std::endl;
		return (1);
	}
	try
	{
	Object obj("./resources/objects/", argv[1]);
	std::vector<Vertex> vertices = obj.get_vertices();
	std::vector<GLuint> indices = obj.get_indices();

	initGlfw();
	GLFWwindow *window = initWindow(WIDTH, HEIGHT, argv[1], NULL, NULL);


	Shader shader("./resources/shaders/default.vert", "./resources/shaders/default.frag");
	GLuint boberTex = load_texture("./resources/textures/bober.bmp", GL_RGB);
	GLuint otherTex = load_texture("./resources/textures/32x32.bmp", GL_RGB);
	shader.use();
	shader.setInt("boberTex", 0);
	shader.setInt("otherTex", 1);



	VAO mainVao;
	mainVao.Bind();

	VBO mainVbo((GLfloat *)vertices.data(), vertices.size() * sizeof(Vertex));
	EBO mainEbo((GLuint *)indices.data(), indices.size() * sizeof(GLuint));

	mainVao.LinkAtr(mainVbo, 0, 3, GL_FLOAT, 8 * sizeof(GLfloat), (void *)0);
	mainVao.LinkAtr(mainVbo, 1, 3, GL_FLOAT, 8 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
	mainVao.LinkAtr(mainVbo, 2, 2, GL_FLOAT, 8 * sizeof(GLfloat), (void *)(6 * sizeof(GLfloat)));
	mainVao.Unbind();

	float angle = 15.0f;
	mlm::vec4 v4(1.0f);

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		// glClearColor(v4.x, v4.y, v4.z, 1.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, boberTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, otherTex);


		float timeValue = glfwGetTime();
		float dimValue = sin(timeValue) / 2.0f + 1.0f;
		mlm::mat4 projection = mlm::perspective(mlm::radians(FOV), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		mlm::mat4 model(1.0f);
		model = mlm::translate(model, mlm::vec3(0.0f, 0.0f, -20.f));
		model = mlm::scale(model, mlm::vec3(5.f));

		model = mlm::rotate(model, mlm::radians(90.0f), mlm::vec3(0.0f, 1.0f, 0.0f));
		model = mlm::rotate(model, mlm::radians(angle / 2), mlm::vec3(0.0f, 1.0f, 1.0f));
		// m = mlm::scale(m, mlm::vec3(dimValue));
		// m = mlm::rotate(m, mlm::radians(angle), mlm::vec3(1.0f, 1.0f, 0.0f));
		v4 = mlm::normalize(model[0]);
		angle += 10.0f;
		shader.setFloat("dim", dimValue);
		shader.setMat4("rotate", model);
		shader.setMat4("projection", projection);
		shader.setInt("texMode", texMode);
		mainVao.Bind();
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	mainEbo.Delete();
	mainVbo.Delete();
	mainVao.Delete();
	shader.Delete();
	delete_texture(boberTex);
	delete_texture(otherTex);
	// glfwTerminate(); // Can cause leaks occasionally
	}
	catch(const std::exception& e)
	{
	}
	return (0);
}
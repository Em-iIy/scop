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

	mlm::vec4 v4(1.0f, 1.0f, 0.0f, 0.0f);
	v4 = mlm::normalize(v4);
	if (argc != 2)
	{
		std::cerr << "Missing input files!" << std::endl;
		return (1);
	}
	initGlfw();

	GLFWwindow *window = initWindow(1000, 1000, argv[1], NULL, NULL);

	Shader shader("./resources/shaders/default.vert", "./resources/shaders/default.frag");

	// Create simple triangle vertices
	GLfloat vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};
	GLuint indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3	// second triangle
	};  

	GLuint boberTex = load_texture("./resources/textures/bober.bmp", GL_RGB);
	GLuint otherTex = load_texture("./resources/textures/32x32.bmp", GL_RGB);
	std::cout << "bobertex: " << boberTex << "\tothertex: " << otherTex << std::endl;
	shader.use();
	shader.setInt("boberTex", 0);
	shader.setInt("otherTex", 1);
	VAO mainVao;
	mainVao.Bind();

	VBO mainVbo(vertices, sizeof(vertices));
	EBO mainEbo(indices, sizeof(indices));
	mainVao.LinkAtr(mainVbo, 0, 3, GL_FLOAT, 8 * sizeof(GLfloat), (void *)0);
	mainVao.LinkAtr(mainVbo, 1, 3, GL_FLOAT, 8 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
	mainVao.LinkAtr(mainVbo, 2, 2, GL_FLOAT, 8 * sizeof(GLfloat), (void *)(6 * sizeof(GLfloat)));
	mainVao.Unbind();
	float angle = 15.0f;
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(v4.x, v4.y, v4.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, boberTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, otherTex);


		float timeValue = glfwGetTime();
		float dimValue = sin(timeValue) / 2.0f + 1.0f;
		mlm::mat4 m(1.0f);
		m = mlm::scale(m, mlm::vec3(dimValue));
		// m = mlm::rotate(m, mlm::radians(angle), mlm::vec3(1.0f, -1.0f, -3.0f));
		m = mlm::rotate(m, mlm::radians(angle), mlm::vec3(1.0f, 1.0f, 0.0f));
		v4 = mlm::normalize(m[0]);
		angle += 1.0f;
		shader.setFloat("dim", dimValue);
		shader.setMat4("rotate", m);
		shader.setInt("texMode", texMode);
		mainVao.Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	mainEbo.Delete();
	mainVbo.Delete();
	mainVao.Delete();
	shader.Delete();
	delete_texture(boberTex);
	delete_texture(otherTex);
	glfwTerminate();
	return (0);
}
/*
Created by: Emily (Em_iIy) Winnink
Created on: 07/09/2024
*/
#pragma once

#include <GLAD/glad.h>
#include <vector>

class VBO
{
public:
	// Reference ID of the Vertex Buffer Object
	GLuint ID;

	VBO(void);
	// Constructor that generates a Vertex Buffer Object and links it to vertices

	VBO(GLfloat* vertices, GLsizeiptr size);
	void Update(GLfloat *vertices, GLsizeiptr size);
	// Binds the VBO
	void Bind();
	// Unbinds the VBO
	void Unbind();
	// Deletes the VBO
	void Delete();
};

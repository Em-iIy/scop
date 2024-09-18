/*
Created by: Emily (Em_iIy) Winnink
Created on: 07/09/2024
*/

#include "VAO.hpp"

// Constructor that generates a VAO ID
VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}

// Links a VBO to the VAO using a certain layout
void VAO::LinkAtr(VBO& VBO, GLuint layout, GLuint n, GLenum type, GLsizeiptr stride, void *offset)
{
	VBO.Bind();
	glVertexAttribPointer(layout, n, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

// Binds the VAO
void VAO::Bind()
{
	glBindVertexArray(ID);
}

// Unbinds the VAO
void VAO::Unbind()
{
	glBindVertexArray(0);
}

// Deletes the VAO
void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}
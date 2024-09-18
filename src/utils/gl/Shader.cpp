/*
Created by: Emily (Em_iIy) Winnink
Created on: 07/09/2024
*/

#include <stdlib.h>
#include <GLAD/glad.h>
#include <iostream>
#include <emlm/emlm.hpp>

#include "Shader.hpp"
#include "utils/utils.hpp"

Shader::Shader(const char *vertexFileName, const char *fragmentFileName)
{
	int		success;
	char	infoLog[512];

	char *vertexSource = read_file(vertexFileName);
	if (!vertexSource)
		return ;
	char *fragmentSource = read_file(fragmentFileName);
	if (!fragmentSource)
	{
		free(vertexSource);
		return ;
	}
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	this->_id = glCreateProgram();
	glAttachShader(this->_id, vertexShader);
	glAttachShader(this->_id, fragmentShader);
	glLinkProgram(this->_id);
	glGetProgramiv(this->_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->_id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
	free(vertexSource);
	free(fragmentSource);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
	// glDeleteProgram(this->_id);
}

void	Shader::use()
{
	glUseProgram(this->_id);
}

void	Shader::Delete()
{
	glDeleteProgram(this->_id);
}

void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(this->_id, name.c_str()), (int)value);
}

void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(this->_id, name.c_str()), value);
}

void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(this->_id, name.c_str()), value);
}

void Shader::setMat4(const std::string &name, mlm::mat4 &value) const
{
	glUniformMatrix4fv(glGetUniformLocation(this->_id, name.c_str()), 1, GL_FALSE, &(value[0][0]));
}



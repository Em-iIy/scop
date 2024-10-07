/*
Created by: Emily (Em_iIy) Winnink
Created on: 09/09/2024
*/

#include <iostream>
#include <GLAD/glad.h>
#include "bmp/bmp.h"


uint	load_texture(const char *img)
{
	uint	texture;
	bmp_t	bmp;
	int		format;

	// Generate and bind texture
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// Specify how the texture will wrap when going out of it's bounds
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Specify how the texels get sampled when it's very close or very far
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load in the image
	bmp = load_bmp(img);
	if (!bmp.data)
	{
		std::cout << "load_texture: could not load: " << img << std::endl;
		exit(EXIT_FAILURE);
	}
	if (bmp.pixel_size == 3)
	{
		format = GL_RGB;
	}
	else
	{
		format = GL_RGBA;
	}

	// Assign the image to the generated texture
	glTexImage2D(GL_TEXTURE_2D, 0, format, bmp.width, bmp.height, 0, format, GL_UNSIGNED_BYTE, bmp.data);

	// Generate mipmap for the texture
	glGenerateMipmap(texture);

	free_bmp(bmp);
	return (texture);
}

void	delete_texture(uint texture)
{
	glDeleteTextures(1, &texture);
}

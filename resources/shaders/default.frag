#version 330 core
out vec4 FragColor;

uniform sampler2D	boberTex;
uniform sampler2D	otherTex;
uniform float		dim;
uniform int			texMode;

in vec3	color;
in vec2	tex;

void main()
{
	if (texMode % 2 == 0)
	{
		FragColor = mix(texture(boberTex, tex), texture(otherTex, tex), 0.2f);
	}
	else
	{
		FragColor = vec4(color * sin(tex.x * 40.0f), 1.0f);
		FragColor.g = cos(tex.y * 20.0f);
	}
	FragColor = vec4(color, 1.0f);
}

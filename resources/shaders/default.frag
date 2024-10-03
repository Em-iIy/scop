#version 330 core
out vec4 FragColor;

uniform sampler2D	boberTex;
uniform sampler2D	otherTex;
uniform float		dim;
uniform float		texMix;
uniform int			texMode;

in vec3	color;
in vec3	normal;
in vec2	tex;

void main()
{
	FragColor = mix(vec4(color, 1.0f), vec4(normal, 1.0f), 0.4f);
	FragColor = mix(FragColor, texture(boberTex, tex), texMix);
}

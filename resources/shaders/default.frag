#version 330 core
out vec4 FragColor;

uniform sampler2D	main_tex;
uniform float		texMix;

in vec3	color;
in vec3	normal;
in vec2	tex;

void main()
{
	vec4 texColor = texture(main_tex, tex);
	FragColor = mix(vec4(color, 1.0f), vec4(normal, 1.0f), 0.4f);
	FragColor = mix(FragColor, texColor, texMix);
	if (FragColor.a < 0.1f)
		discard;
}

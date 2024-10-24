#version 330 core
out vec4 FragColor;

uniform sampler2D	tex0;
uniform float		tex_mix;

in vec3	color;
in vec3	normal;
in vec2	tex;

void main()
{
	vec4 tex_color = texture(tex0, tex);
	FragColor = mix(vec4(color, 1.0f), vec4(normal, 1.0f), 0.4f);
	FragColor = mix(FragColor, tex_color, tex_mix);
	if (FragColor.a < 0.1f)
		discard;
}

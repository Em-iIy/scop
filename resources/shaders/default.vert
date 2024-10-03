#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTex;

out vec3	color;
out vec3	normal;
out vec2	tex;

uniform mat4	rotate;
uniform mat4	projection;

void main()
{
	gl_Position = projection * rotate * vec4(aPos, 1.0);
	color = aColor;
	normal = aNormal;
	tex = aTex;
}

#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;

out vec3	color;
out vec2	tex;

uniform mat4	rotate;

void main()
{
	gl_Position = rotate * vec4(aPos, 1.0);
	color = aColor;
	tex = aTex;
	// tex = vec2(aPos.x, aPos.y);
}

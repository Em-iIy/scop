#include <cmath>

#include "../mlm/mat4.hpp"

namespace mlm {

// perspective
// http://www.songho.ca/opengl/gl_projectionmatrix.html#perspective
// https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/building-basic-perspective-projection-matrix.html
mat4	perspective(const float &fov, const float &near, const float &far)
{
	float scale = 1 / tan(fov * 0.5 * M_PI / 180);
	mlm::mat4 projection(
		scale, 0.0f, 0.0f, 0.0f,
		0.0f, scale, 0.0f, 0.0f,
		0.0f, 0.0f, -((far + near) / (far - near)), -1.0f,
		0.0f, 0.0f, -((far * near) / (far - near)), 0.0f
	);
	return (projection);
}

}
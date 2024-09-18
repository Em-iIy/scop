#include <cmath>

#include "../mlm/mat4.hpp"
#include "../mlm/vec3.hpp"
#include "../mlm/normalize.hpp"

namespace mlm {

// rotate
// https://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
// [ cosθ + v.x^2 * (1 - cosθ)				v.x * v.y * (1 - cosθ) - v.z * sinθ		v.x * v.z * (1 - cosθ) + v.x * sinθ		0 ]
// [ v.x * v.y * (1 - cosθ) + v.z * sinθ	cosθ + v.y^2 * (1 - cosθ)				v.x * v.z * (1 - cosθ) - v.x * sinθ		0 ]
// [ v.x * v.z * (1 - cosθ) - v.x * sinθ	v.x * v.z * (1 - cosθ) + v.x * sinθ		cosθ + v.z^2 * (1 - cosθ)				0 ]
// [ 0										0										0										1 ]
mat4	rotate(const mat4 &m, const float &θ, const vec3 &v)
{
	float	cosθ = cosf(θ);
	float	sinθ = sinf(θ);

	vec3	axis = normalize(v);
	vec3	tempSin(sinθ * axis);
	vec3	tempCos((1 - cosθ) * axis);

	return {
		m * mat4(
			vec4(cosθ + axis.x * tempCos.x, axis.x * tempCos.y - tempSin.z, axis.x * tempCos.z + tempSin.y, 0.0f),
			vec4(axis.x * tempCos.y + tempSin.z, cosθ + axis.y * tempCos.y, axis.y * tempCos.z - tempSin.x, 0.0f),
			vec4(axis.x * tempCos.z - tempSin.y, axis.y * tempCos.z + tempSin.x, cosθ + axis.z * tempCos.z, 0.0f),
			vec4(0.0f, 0.0f, 0.0f, 1.0f)
		)
	};
}

// translate
// 	[	1	0	0	v.x	]
// 	[	0	1	0	v.y	]
// 	[	0	0	1	v.z	]
// 	[	0	0	0	1	]
mat4	translate(const mat4 &m, const vec3 &v)
{
	mat4 ret(m);
	ret[3] = v.x * m[0] + v.y * m[1] + v.z * m[2] + m[3];
	return (ret);
}

// scale
mat4	scale(const mat4 &m, const vec3 &v)
{
	return (mat4(
		m[0] * v.x,
		m[1] * v.y,
		m[2] * v.z,
		m[3]
	));
}

// lookat

}

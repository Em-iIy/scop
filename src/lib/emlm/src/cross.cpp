#include "../mlm/vec3.hpp"

namespace mlm {

vec3	cross(const vec3 &v1, const vec3 &v2)
{
	return (vec3(
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	));
}

}

#ifndef TONKYTIN_MATHUTILS_HPP
#define TONKYTIN_MATHUTILS_HPP

#include <cstdint>
#include <glm/glm.hpp>

namespace tonkytin {

constexpr float PI = M_PIf32;

using Point    = glm::highp_vec3;
using Vec3     = glm::highp_vec3;
using Mat4x4   = glm::highp_mat4x4;
using PixColor = glm::highp_vec4;

class Ray {
 public:
	Point mOrigin;
	Vec3 mDirection;

	inline Point pointAtDist(float t) { return mOrigin + t * mDirection; }
};

}  // namespace tonkytin

#endif  // TONKYTIN_MATH_UTILS_HPP

#ifndef TONKYTIN_MATHUTILS_HPP
#define TONKYTIN_MATHUTILS_HPP

#include <cstdint>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

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

	std::string toString() const
	{
		nlohmann::json j = {"Ray",
		                    {"origin",
		                     {{"x", mOrigin[0]}, {"y", mOrigin[1]}, {"z", mOrigin[2]}},

		                     {"direction", {{"x", mDirection[0]}, {"y", mDirection[1]}, {"z", mDirection[2]}}}}};
		return to_string(j);
	}
};

inline PixColor
interpolate(float u, float v, const PixColor& c1, const PixColor& c2, const PixColor& c3, const PixColor& c4)
{
	return (1 - u) * (1 - v) * c1 + u * (1 - v) * c2 + u * v * c3 + (1 - u) * v * c4;
}

}  // namespace tonkytin

#endif  // TONKYTIN_MATH_UTILS_HPP

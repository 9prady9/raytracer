#ifndef TONKYTIN_PERLINNOISE_HPP
#define TONKYTIN_PERLINNOISE_HPP

#include <array>
#include <concepts>
#include <cstdint>

#include "math_utils.hpp"

namespace tonkytin {

constexpr uint32_t PerlinPatchDim = 30;

template< typename T >
requires std::floating_point< T >
class PerlinNoise {
 private:
	std::array< T, PerlinPatchDim * PerlinPatchDim > mPerlinPatchX;
	std::array< T, PerlinPatchDim * PerlinPatchDim > mPerlinPatchY;
	PixColor mColor;

 public:
	PerlinNoise();
	[[nodiscard]] T noise2D(T u, T v);
	[[nodiscard]] PixColor getNoiseColor() const { return mColor; }
};

}  // namespace tonkytin

#endif  // TONKYTIN_PERLINNOISE_HPP

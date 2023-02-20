#include "perlin_noise.hpp"

#include <algorithm>
#include <cmath>
#include <random>

namespace tonkytin {

template< typename T >
requires std::floating_point< T > PerlinNoise< T >::PerlinNoise()
: mColor(0.361, 0.298, 0.102, 0.0)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution< T > dis(0.0, 1.0);
	std::generate(mPerlinPatchX.begin(), mPerlinPatchX.end(), [&]() { return dis(gen); });
	std::generate(mPerlinPatchY.begin(), mPerlinPatchY.end(), [&]() { return dis(gen); });
}

template< typename T >
requires std::floating_point< T > T PerlinNoise< T >::noise2D(T u, T v)
{
	constexpr T factor = static_cast< T >(PerlinPatchDim - 1);

	T Iv, Jv;
	T X    = factor * u;
	T Y    = factor * v;
	T i    = std::modf(X, &Iv);
	T j    = std::modf(Y, &Jv);
	auto I = static_cast< int32_t >(Iv);
	auto J = static_cast< int32_t >(Jv);

	auto gX          = [&](int32_t x, int32_t y) { return mPerlinPatchX[x + y * PerlinPatchDim]; };
	auto gY          = [&](int32_t x, int32_t y) { return mPerlinPatchY[x + y * PerlinPatchDim]; };
	auto interpolate = [](T a) { return a * a * (3 - 2 * a); };

	T C00      = gX(I, J) * i + gY(I, J) * j;
	T C10      = gX(I + 1, J) * (i - 1) + gY(I + 1, J) * j;
	T C01      = gX(I, J + 1) * i + gY(I, J + 1) * (j - 1);
	T C11      = gX(I + 1, J + 1) * (i - 1) + gY(I + 1, J + 1) * (j - 1);
	T alpha_i0 = (1 - interpolate(i)) * C00 + interpolate(i) * C10;
	T alpha_i1 = (1 - interpolate(i)) * C01 + interpolate(i) * C11;
	T alpha    = alpha_i0 * (1 - interpolate(j)) + alpha_i1 * interpolate(j);

	return alpha + 0.5;
}

#define INSTANTIATE(TYPE)                                 \
	template PerlinNoise< TYPE >::PerlinNoise();            \
	template TYPE PerlinNoise< TYPE >::noise2D(TYPE, TYPE); \
	template PixColor PerlinNoise< TYPE >::getNoiseColor() const;

INSTANTIATE(float)

}  // namespace tonkytin

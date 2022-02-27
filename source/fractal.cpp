#include "fractal.hpp"

namespace tonkytin {

template< typename T >
requires std::floating_point< T >
bool Fractal< T >::isInside(T realValue, T imagValue, uint32_t Iterations, T Threshold)
{
	// Transform complex number into (-1,1) range
	realValue = 2 * (realValue - 0.5);
	imagValue = 2 * (imagValue - 0.5);
	for (unsigned int iteration = 0; iteration < Iterations; iteration++) {
		auto r = (realValue * realValue - imagValue * imagValue) + mRealValue;
		auto i = 2 * realValue * imagValue + mImaginaryValue;
		if ((r * r + i * i) > Threshold) { return false; }
		realValue = r;
		imagValue = i;
	}
	return true;
}

}  // namespace tonkytin
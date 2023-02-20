#include "fractal.hpp"

#include "nlohmann/json.hpp"

namespace tonkytin {

template< typename T >
requires std::floating_point< T >
bool Fractal< T >::isInside(T realValue, T imagValue, const uint32_t Iterations, const T Threshold)
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

template< typename T >
requires std::floating_point< T > std::string Fractal< T >::toString()
const
{
	nlohmann::json json;
	json["real"] = mRealValue;
	json["imag"] = mImaginaryValue;
	return std::move(to_string(json));
}

#define INSTANTIATE(TYPE)                                                          \
	template bool Fractal< TYPE >::isInside(TYPE, TYPE, const uint32_t, const TYPE); \
	template std::string Fractal< TYPE >::toString() const;

INSTANTIATE(float);

}  // namespace tonkytin
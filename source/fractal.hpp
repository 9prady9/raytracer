#ifndef TONKYTIN_FRACTAL_HPP
#define TONKYTIN_FRACTAL_HPP

#include <concepts>
#include <cstdint>
#include <ostream>

namespace tonkytin {

template< typename T >
requires std::floating_point< T >
class Fractal {
 public:
	/// Ensure FractalIterationCount and ComputationThreshold doesn't change across region where a
	/// given fractal is being generated
	bool isInside(T real, T img, const uint32_t FractalIterationCount = 200, const T ComputationThreshold = 1e3);

	std::string toString() const;

	// Fractal Attributes
	T mRealValue;
	T mImaginaryValue;
};

}  // namespace tonkytin

#endif  // TONKYTIN_FRACTAL_HPP
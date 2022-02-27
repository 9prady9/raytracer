#ifndef TONKYTIN_FRACTAL_HPP
#define TONKYTIN_FRACTAL_HPP

#include <concepts>
#include <cstdint>

namespace tonkytin {

template< typename T >
requires std::floating_point< T >
class Fractal {
 private:
	T mRealValue;
	T mImaginaryValue;

 public:
	/// Ensure FractalIterationCount and ComputationThreshold doesn't change across region where a
	/// given fractal is being generated
	bool isInside(T real, T img, uint32_t FractalIterationCount = 200, T ComputationThreshold = 1e3);
};

}  // namespace tonkytin

#endif  // TONKYTIN_FRACTAL_HPP
#include "material.hpp"

// #define STB_IMAGE_MANIPULATION // defined via cmake target_compile_definition
#include <cmath>
#include <iostream>

#include "math_utils.hpp"
#include "stb_image.h"

namespace tonkytin {

template< typename T >
requires std::floating_point< T >
void Material< T >::readTexture()
{
	if (mTextureKind != TextureKind::IMAGE) { return; }
	int c       = 0;
	auto buffer = stbi_load(mTextureFileLocation.c_str(), &mTextureCols, &mTextureRows, &c, 4);

	const auto nElems = mTextureCols * mTextureRows;

	mTexels.reserve(nElems);
	for (int32_t i = 0; i < nElems; ++i) {
		mTexels.template emplace_back(buffer[i], buffer[i + 1], buffer[i + 2], buffer[i + 3]);
		buffer += 4;  // A char per component in a texel
	}
}

template< typename T >
requires std::floating_point< T > PixColor Material< T >::computeTextureColor(T u, T v)
{
	if (mTextureKind == TextureKind::IMAGE) {
		if (abs(u - 1.0) < 1.0e-6) u = 0.0;
		if (abs(v - 1.0) < 1.0e-6) v = 0.0;
		float X = u * (mTextureRows - 1);
		float Y = v * (mTextureCols - 1);
		int i   = (int)X;
		int j   = (int)Y;
		u       = X - i;
		v       = Y - j;
		auto get = [&](int col, int row) { return mTexels[col + row * mTextureRows]; };
		return interpolate(u, v, get(i, j), get(i + 1, j), get(i + 1, j + 1), get(i, j + 1));
	} else if (mTextureKind == TextureKind::PROCEDURE) {
		if (abs(u - 1.0) < 1.0e-6) u = 0.0;
		if (abs(v - 1.0) < 1.0e-6) v = 0.0;

		// ( u-0.5)*(u-0.5)+(v-0.5)*(v-0.5) < 0.15
		// (u < 0.5 && v < 0.5) || (u > 0.5 && v > 0.5)

		/*float fraction, intpart;
		fraction = modf(noise->noise2D(u,v), &intpart);
		return (fraction*noise->getNoiseColor() + (1-fraction)*this->colour);*/

		if (mFractalGenerator->isInside(u, v)) {
			return {0.0, 0.0, 1.0, 0.0};
		} else {
			return {1.0, 1.0, 1.0, 0.0};
		}
	} else {
		std::cerr << "Texture is not enables, some wrong call occured, chk code\n";
		PixColor ret_val(0.0, 0.0, 0.0, 0.0);
		return ret_val;
	}
}

template< typename T >
requires std::floating_point< T > PixColor Material< T >::computeSolidTextureColor(const Point &Ph)
{
	PixColor RED(0.0, 0.0, 1.0, 0.0);
	PixColor WHITE(1.0, 1.0, 1.0, 0.0);

	/*if( JUMP(Ph.X()) )
	    return RED;
	else
	    return WHITE;*/

	/*if( JUMP2(Ph.X(),Ph.Y()) )
	    return RED;
	else
	    return WHITE;*/

	/*if( RINGS(Ph.X(),Ph.Y(),Ph.Z()) )
	    return RED;
	else
	    return WHITE;*/

	float xFrac, xInt;
	xFrac = std::modf(Ph.x, &xInt);
	float yFrac, yInt;
	yFrac = std::modf(Ph.y, &yInt);

	if (mFractalGenerator->isInside(abs(xFrac), abs(yFrac)))
		return RED;
	else
		return WHITE;
}

#define INSTANTIATE(TYPE)                                                                                      \
	template Material< TYPE >::Material(TYPE, const PixColor &, MaterialKind, TextureKind, const std::string &); \
	template void Material< TYPE >::readTexture();                                                               \
	template PixColor Material< TYPE >::computeTextureColor(TYPE, TYPE);                                         \
	template PixColor Material< TYPE >::computeSolidTextureColor(const Point &);

INSTANTIATE(float)

}  // namespace tonkytin

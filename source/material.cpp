#include "material.hpp"

#include <cmath>
#include <iostream>

namespace tonkytin {

template< typename T >
requires std::floating_point< T >
void Material< T >::readTexture()
{
	if (mTextureKind == TextureKind::IMAGE) {
		unsigned int I, J;
		QColor c;
		QImage texture(TextureFile.c_str());
		if (texture.isNull()) { std::cerr << " Given texture is not read properly; Check file path." << std::endl; }
		this->mTextureRows = texture.height();
		this->mTextureCols = texture.width();

		mTexels = new PixColor *[mTextureRows];
		for (I = 0; I < mTextureRows; I++) mTexels[I] = new PixColor[mTextureCols];

		for (I = 0; I < mTextureRows; I++) {
			for (J = 0; J < mTextureCols; J++) {
				c = texture.pixel(I, J);
				mTexels[I][J].set(c.redF(), c.greenF(), c.blueF(), c.alphaF());
			}
		}
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
		PixColor ret_val;
		bilinearInterpolation(ret_val, u, v, mTexels[i][j], mTexels[i + 1][j], mTexels[i + 1][j + 1], mTexels[i][j + 1]);
		return ret_val;
	} else if (mTextureKind == TextureKind::PROCEDURE) {
		if (abs(u - 1.0) < 1.0e-6) u = 0.0;
		if (abs(v - 1.0) < 1.0e-6) v = 0.0;

		// ( u-0.5)*(u-0.5)+(v-0.5)*(v-0.5) < 0.15
		// (u < 0.5 && v < 0.5) || (u > 0.5 && v > 0.5)

		/*float fraction, intpart;
		fraction = modf(noise->noise2D(u,v), &intpart);
		return (fraction*noise->getNoiseColor() + (1-fraction)*this->colour);*/

		if (mFractalGenerator->isInside(u, v)) {
			PixColor ret_val(0.0, 0.0, 1.0, 0.0);
			return ret_val;
		} else {
			PixColor ret_val(1.0, 1.0, 1.0, 0.0);
			return ret_val;
		}
		// return this->colour;
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

}  // namespace tonkytin

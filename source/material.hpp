#ifndef TONKYTIN_MATERIAL_HPP
#define TONKYTIN_MATERIAL_HPP

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "fractal.hpp"
#include "perlin_noise.hpp"

namespace tonkytin {

enum class MaterialKind : uint32_t { DIFFUSIVE, REFLECTIVE, REFRACTIVE, EMISSIVE };

enum class TextureKind : uint32_t { NONE, IMAGE, PROCEDURE, SOLID };

template< typename T >
requires std::floating_point< T >
class Material {
 private:
	std::unique_ptr< Fractal< T > > mFractalGenerator;
	std::unique_ptr< PerlinNoise< T > > mNoiseGenerator;

 public:
	// Attributes
	T mRefractiveIndex;
	PixColor mColour;
	MaterialKind mMaterialKind;
	TextureKind mTextureKind;
	std::string mTextureFileLocation;
	std::vector< PixColor > mTexels;
	uint32_t mTextureRows;
	uint32_t mTextureCols;

	Material(T mRefractiveIndex, const PixColor &mColour, MaterialKind mMaterialKind, TextureKind mTextureKind,
	         const std::string &mTextureFileLocation)
	: mRefractiveIndex(mRefractiveIndex)
	, mColour(mColour)
	, mMaterialKind(mMaterialKind)
	, mTextureKind(mTextureKind)
	, mTextureFileLocation(mTextureFileLocation)
	{
		readTexture();
		mFractalGenerator = std::make_unique< Fractal< T > >(-0.8, 0.156);
		mNoiseGenerator   = std::make_unique< PerlinNoise< T > >();
	}

	void readTexture();
	PixColor computeTextureColor(T u, T v);
	PixColor computeSolidTextureColor(const Point &point);
};

}  // namespace tonkytin

#endif  // TONKYTIN_MATERIAL_HPP

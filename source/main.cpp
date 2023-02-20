#include <CLI11.hpp>
#include <iostream>
#include <string>

#include "fractal.hpp"
#include "material.hpp"
#include "perlin_noise.hpp"

int main(int argc, char* argv[])
{
	CLI::App app{"tonkytin: a ray tracer"};

	// std::string filename = "help_string_dummy_default";
	// app.add_option("-h,--help", filename, "A help string");

	CLI11_PARSE(app, argc, argv);

	[[maybe_unused]] tonkytin::Fractal< float > fractal{1.0, -0.5};

	[[maybe_unused]] tonkytin::PerlinNoise< float > noiseGenerator;

	[[maybe_unused]] tonkytin::Material< float > material(1.0, {1.0, 1.0, 1.0, 1.0}, tonkytin::MaterialKind::DIFFUSIVE,
	                                                      tonkytin::TextureKind::IMAGE,
	                                                      std::string(TONKYTIN_ASSETS_ROOT) + "textures/brownpaper.jpg");

	std::cout << fractal.toString() << std::endl;
	std::cout << tonkytin::Ray{glm::vec3(1), glm::vec3(1)}.toString() << std::endl;
	return 0;
}

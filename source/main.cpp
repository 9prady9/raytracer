#include <CLI11.hpp>
#include <glm/glm.hpp>

int main(int argc, char* argv[])
{
	CLI::App app{"tonkytin: a ray tracer"};

	// std::string filename = "help_string_dummy_default";
	// app.add_option("-h,--help", filename, "A help string");

	CLI11_PARSE(app, argc, argv);

	return 0;
}

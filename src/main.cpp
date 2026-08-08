#include "basic_shapes.hpp"
#include <cstdlib>
#include <cxxopts.hpp>
#include <functional>
#include <iostream>
#include <map>
#include <string>

namespace {

using DemoFn = std::function<int()>;

const std::map<unsigned int, DemoFn>& demoRegistry()
{
    static const std::map<unsigned int, DemoFn> registry{
        {1, basic_shapes::showEquilateralTriangle},
        {2, basic_shapes::showEquilateralTriangleWithInscribedUpsideDownEqTriangle},
    };
    return registry;
}
} // namespace

int main(int argc, char* argv[])
{
    cxxopts::Options options("Experimental OpenGL", "Usage");
    options.add_options()("h,help", "Print usage and documentation")(
        "n,num", "Example number to run", cxxopts::value<unsigned int>());

    try {
        const auto result = options.parse(argc, argv);

        if (result.count("help")) {
            std::cout << options.help() << std::endl;
            return 0;
        }

        const auto number = result["n"].as<unsigned int>();

        const auto& registry = demoRegistry();
        const auto it = registry.find(number);

        if (it == registry.end()) {
            std::cerr << "Unknown demo number: " << number << "\n\n";
            return EXIT_FAILURE;
        }

        return it->second();
    }
    catch (const cxxopts::exceptions::parsing& e) {
        std::cerr << "Error parsing options: " << e.what() << "\n\n";
        std::cerr << options.help() << std::endl;
        return 1;
    }
}

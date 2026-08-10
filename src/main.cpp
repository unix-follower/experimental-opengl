#include "basic_shapes.hpp"
#include <boost/stacktrace.hpp>
#include <cstdlib>
#include <cxxopts.hpp>
#include <filesystem>
#include <functional>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

namespace {

using DemoFn = std::function<int()>;

const std::map<unsigned int, DemoFn>& demoRegistry()
{
    static const std::map<unsigned int, DemoFn> registry{
        {1, basic_shapes::showEquilateralTriangle},
        {2, basic_shapes::showEqTriWithInscribedUpsideDownEqTri},
        {3, basic_shapes::showExampleWithProjectLayout},
    };
    return registry;
}

/**
 * Similar to Java style
 */
void printTrace(const std::string& exception_msg = "An error occurred")
{
    boost::stacktrace::stacktrace st;

    // Java prints the Exception message on the first line
    std::cerr << exception_msg << '\n';

    for (const auto& frame : st) {
        // Skip printing the tracing utility function itself
        if (frame.name().contains("printTrace")) {
            continue;
        }

        // Extract just the file name out of the absolute file path
        std::string file_name = std::filesystem::path(frame.source_file()).filename().string();
        if (file_name.empty()) {
            file_name = "Unknown Source";
        }

        // Format exactly like Java: "\tat method(file:line)"
        std::cerr << "\tat " << frame.name() << "(" << file_name;
        if (frame.source_line() > 0) {
            std::cerr << ":" << frame.source_line();
        }
        std::cerr << ")\n";
    }
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
            std::cout << options.help() << '\n';
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
        std::cerr << options.help() << '\n';
        return 1;
    }
    catch (const std::exception& e) {
        printTrace(e.what());
        return -1;
    }
    catch (...) {
        printTrace("Unknown error");
        return -2;
    }
}

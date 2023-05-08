#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <filesystem>
#include "first_app.hpp"

int main()
{
    std::filesystem::path cwd = std::filesystem::current_path();
    std::cout << cwd << std::endl;

    lve::FirstApp app{};

    try
    {
        app.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

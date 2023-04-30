#include "lve_pipeline.hpp"
#include <fstream>
#include <stdexcept>
#include <iostream>

lve::LvePipeline::LvePipeline(const std::string &vertFilepath, const std::string &fragFilePath)
{
    createGraphicsPipeline(vertFilepath, fragFilePath);
}

std::vector<char> lve::LvePipeline::readFile(const std::string &filepath)
{
    std::ifstream file{filepath, std::ios::ate | std::ios::binary};

    if (!file) {
        throw std::runtime_error("Failed to open file: " + filepath);
    }
    auto fileSize = file.tellg();
    std::vector<char> buffer(static_cast<size_t>(fileSize));
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();
    return buffer;
}

void lve::LvePipeline::createGraphicsPipeline(const std::string &vertFilepath, const std::string &fragFilepath)
{
    auto vertCode = readFile(vertFilepath);
    auto fragCode = readFile(fragFilepath);

    using std::cout, std::endl;
    cout << "Vertex Shader Code Size: " << vertCode.size() << endl;
    cout << "Fragment Shader Code Size: " << fragCode.size() << endl;
}

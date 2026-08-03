#include "jsonfile.hpp"

#include <fstream>
#include <iostream>

namespace utils::files
{

std::optional<nlohmann::json> readFileToJson(const std::filesystem::path& filepath)
{
    using nlohmann::json;

    json readingJson;
    try
    {
        std::ifstream readingFile(filepath);
        if (!readingFile)
        {
            std::cerr << "failed to open " << filepath.filename() << std::endl;
            return std::nullopt;
        }
        readingFile >> readingJson;
    }
    catch (const json::parse_error& e)
    {
        std::cerr << "error parsing json: " << e.what() << std::endl;
        return std::nullopt;
    }
    return readingJson;
}

bool writeJsonToFile(const std::filesystem::path& filepath, const nlohmann::json& jason)
{
    try
    {
        std::ofstream writingFile(filepath);
        if (!writingFile)
        {
            std::cerr << "failed to open " << filepath.filename() << std::endl;
            return false;
        }

        writingFile << std::setw(4) << jason << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "error writing json: " << e.what() << std::endl;
        return false;
    }
    return true;
}

} // namespace utils::files

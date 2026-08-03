#include "directory.hpp"

#include <iostream>

namespace utils::files
{

bool mkDirs(const std::filesystem::path& folderPath)
{
    using namespace std::filesystem;

    try
    {
        create_directories(folderPath);
    }
    catch (const filesystem_error& e)
    {
        std::cerr << "error creating folder " << folderPath.string() << ": " << e.what()
                  << std::endl;
        return false;
    }
    return true;
}

bool rmR(const std::filesystem::path& folderPath)
{
    using namespace std::filesystem;

    try
    {
        remove_all(folderPath);
    }
    catch (const filesystem_error& e)
    {
        std::cerr << "error deleting folder recursively " << folderPath.string() << ": " << e.what()
                  << std::endl;
        return false;
    }
    return true;
}

} // namespace utils::files

#include "safepath.hpp"

#include <stdexcept>

namespace utils::safepath
{

void assertSafePath(const std::filesystem::path& root, const std::filesystem::path& target)
{
    auto normalizedRoot = std::filesystem::weakly_canonical(root);
    auto normalizedPath = std::filesystem::weakly_canonical(target);

    auto rootIt = normalizedRoot.begin();
    auto pathIt = normalizedPath.begin();

    for (; rootIt != normalizedRoot.end(); rootIt++, pathIt++)
    {
        if (pathIt == normalizedPath.end() || *rootIt != *pathIt)
        {
            throw std::runtime_error("Path escaped its root folder: " + target.string());
        }
    }
}

void fastSafePath(const std::filesystem::path& target)
{
    if (target.is_absolute())
        throw std::runtime_error("Absolute path used as target");

    int depth = 0;
    for (const auto& part : target)
    {
        if (part == ".")
            continue;

        if (part == "..")
        {
            if (depth == 0)
                throw std::runtime_error("Path escaped root folder");

            depth--;
        }
        else
        {
            depth++;
        }
    }
}

} // namespace utils::safepath

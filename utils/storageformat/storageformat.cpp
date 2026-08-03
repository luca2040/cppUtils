#include "storageformat.hpp"

#include <format>

std::string utils::formatStorage(uintmax_t size)
{
    constexpr std::array units{"B", "KB", "MB", "GB", "TB", "PB"};

    double      value = static_cast<double>(size);
    std::size_t unit  = 0;

    while (value >= 1000 && unit + 1 < units.size())
    {
        value /= 1000;
        unit++;
    }

    std::string res = {};

    if (unit == 0)
        return std::format("{} {}", size, units[unit]);

    return std::format("{:.2f} {}", value, units[unit]);
}

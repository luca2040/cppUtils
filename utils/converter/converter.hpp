// utils/converter/converter.hpp from https://github.com/luca2040/cppUtils

#pragma once

#include <limits>

namespace utils::converter
{

class Converter
{
  public:
    static bool strToULong(const char* input, unsigned long& outValue);

    template <typename T> static bool strToUnsigned(const char* input, T& outValue)
    {
        unsigned long convertedValue;
        bool          validValue = strToULong(input, convertedValue);
        if (!validValue)
            return false;

        if (convertedValue > std::numeric_limits<T>::max())
            return false;

        outValue = static_cast<T>(convertedValue);
        return true;
    }
};

} // namespace utils::converter

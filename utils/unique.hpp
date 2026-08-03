// adapted from utils/unique.hpp from https://github.com/luca2040/cppUtils

#pragma once

#include <cstdint>
#include <string>

namespace utils
{

template <typename T> class Unique
{
  private:
    size_t currentValue = 0;
    T      baseVal;

  public:
    Unique() = default;
    Unique(T baseVal_) : baseVal(baseVal_) {};

    T newVal()
    {
        if constexpr (std::is_same_v<T, size_t>)
        {
            return currentValue++;
        }
        else if constexpr (std::is_same_v<T, std::string>)
        {
            return baseVal + std::to_string(currentValue++);
        }
        else if constexpr (std::is_same_v<T, uintptr_t>)
        {
            if (!currentValue) // for some reason value 0 breaks it
                currentValue++;
            return currentValue++;
        }
        else
        {
            return baseVal + (currentValue++);
        }
    }
};

} // namespace utils

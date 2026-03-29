#pragma once

#include <string>
#include <type_traits>

namespace utils
{
  template <typename T>
  class Unique
  {
  private:
    size_t currentValue = 0;
    T baseVal;

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
      else
      {
        return baseVal + (currentValue++);
      }
    }
  };
}
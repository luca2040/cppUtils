#pragma once

#include <vector>
#include <algorithm>

namespace utils
{

template <typename T> inline size_t vecContains(const std::vector<T>& vec, const T& val)
{
    return vec.end() - std::find(vec.begin(), vec.end(), val);
}

template <typename T> inline void vecRemove(std::vector<T>& vec, const T& val)
{
    if (size_t pos = vecContains(vec, val))
        vec.erase(vec.end() - pos);
}

} // namespace utils

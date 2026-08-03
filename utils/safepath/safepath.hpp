#pragma once

#include <filesystem>

namespace utils::safepath
{

// those throw exceptions when the path is unsafe
void assertSafePath(const std::filesystem::path& root, const std::filesystem::path& target);
void fastSafePath(const std::filesystem::path& target);

} // namespace utils::safepath

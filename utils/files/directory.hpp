#pragma once

#include <filesystem>

namespace utils::files
{

bool mkDirs(const std::filesystem::path& folderPath);
bool rmR(const std::filesystem::path& folderPath);

} // namespace utils::files
